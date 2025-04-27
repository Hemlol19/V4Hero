#define SDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Altar.h"
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../StateManager.h"


AltarMenu::AltarMenu()
{
    SPDLOG_INFO("Initializing Altar...");

    Config* config = CoreManager::getInstance().getConfig();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

    int quality = config->GetInt("textureQuality");
    q = quality;

    switch (quality)
    {
        case 0: ///low
        {
            ratio_x = config->GetInt("resX") / float(640);
            ratio_y = config->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratio_x = config->GetInt("resX") / float(1280);
            ratio_y = config->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratio_x = config->GetInt("resX") / float(1920);
            ratio_y = config->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratio_x = config->GetInt("resX") / float(3840);
            ratio_y = config->GetInt("resY") / float(2160);
            break;
        }
    }

    res_ratio_x = config->GetInt("resX") / float(1280);
    res_ratio_y = config->GetInt("resY") / float(720);



    altar_main.loadFromFile("resources/graphics/ui/altar/altar_main.png", quality);

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    altar_title.defaultStyleSetFont(font);
    altar_title.defaultStyleSetCharSize(40);
    altar_title.defaultStyleSetColor(sf::Color(111, 71, 51, 255));
    altar_title.append(Func::GetStrFromKey("altar_title"));

    altar_kaching.defaultStyleSetFont(font);
    altar_kaching.defaultStyleSetCharSize(30);
    altar_kaching.defaultStyleSetColor(sf::Color(111,71,51,255));
    altar_kaching.append(Func::GetStrFromKey("altar_title"));

    altar_item_title.defaultStyleSetFont(font);
    altar_item_title.defaultStyleSetCharSize(24);
    altar_item_title.defaultStyleSetColor(sf::Color(111,71,51,255));
    altar_item_title.append("");

    altar_item_category.defaultStyleSetFont(font);
    altar_item_category.defaultStyleSetCharSize(20);
    altar_item_category.defaultStyleSetColor(sf::Color(111,71,51,255));
    altar_item_category.append("");

    altar_item_desc.defaultStyleSetFont(font);
    altar_item_desc.defaultStyleSetCharSize(25);
    altar_item_desc.defaultStyleSetColor(sf::Color(111,71,51,255));
    altar_item_desc.append("");

    ctrlTips.create(54*3, font, 20, sf::String("Left/Right/Up/Down: Navigate      O: Exit to Patapolis"), quality);

    SPDLOG_INFO("Initializing Altar finished.");
}

void AltarMenu::updateAltarDescriptions()
{
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    int selItem = (grid_sel_y + grid_offset_y) * 4 + grid_sel_x;

    if (selItem < inventory_boxes.size())
    {
        altar_item_title.reset();
        altar_item_title.append(Func::GetStrFromKey(inventory_boxes[selItem].data->item_name));
        altar_item_category.reset();
        altar_item_category.append(Func::GetStrFromKey("altar_category_" + inventory_boxes[selItem].data->item_category));
        altar_item_desc.reset();
        altar_item_desc.append(Func::ConvertToUtf8String(Func::wrap_text(inventory_boxes[selItem].data->item_description, 420*3, font, 26)));
    } else
    {
        altar_item_title.reset();
        altar_item_title.append("");
        altar_item_category.reset();
        altar_item_category.append("");
        altar_item_desc.reset();
        altar_item_desc.append("");
    }
}

void AltarMenu::reloadInventory()
{
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();

    SPDLOG_DEBUG("AltarMenu::reloadInventory();");

    std::vector<InvBox> old_invboxes = inventory_boxes; ///for comparison and new item highlight

    inventory_boxes.clear();

    SPDLOG_INFO("Fetching inventory entries: {} entries", saveReader->invData.items.size());
    for (int i = 0; i < saveReader->invData.items.size(); i++)
    {
        Item* cur_item = saveReader->invData.items[i].item;

        bool ex = false;

        if (!ex)
        {
            InvBox tmp;
            tmp.data = cur_item;
            tmp.amount = saveReader->invData.items[i].item_count;

            tmp.r_outer.setSize(sf::Vector2f(104.0 * res_ratio_x, 77.0 * res_ratio_y));
            tmp.r_outer.setFillColor(sf::Color(102, 102, 102, 255));

            tmp.r_inner.setSize(sf::Vector2f(72.0 * res_ratio_x, 72.0 * res_ratio_y));
            tmp.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

            switch (saveReader->itemReg.getCategoryIDByString(cur_item->item_category))
            {
                case 1: ///Materials
                {
                    tmp.r_inner.setFillColor(sf::Color(146, 173, 217, 255));

                    ///look up material's icon
                    //tmp.icon.loadFromFile("resources/graphics/ui/altar/materials/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png", q);
                    tmp.icon.loadFromFile("resources/graphics/item/textures/" + cur_item->spritesheet + "/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png", q);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().size.x / 2, tmp.icon.getLocalBounds().size.y / 2);

                    break;
                }

                case 3: ///Weapons
                {
                    tmp.r_inner.setFillColor(sf::Color(199, 221, 167, 255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/ui/altar/equip/spear_1.png", q);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().size.x / 2, tmp.icon.getLocalBounds().size.y / 2);

                    break;
                }

                case 4: ///Helmets
                {
                    tmp.r_inner.setFillColor(sf::Color(199, 221, 167, 255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/ui/altar/equip/helm_1.png", q);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().size.x / 2, tmp.icon.getLocalBounds().size.y / 2);

                    break;
                }

                case 0: ///Key items
                {
                    tmp.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

                    ///look up material's icon
                    tmp.icon.loadFromFile("resources/graphics/item/textures/" + cur_item->spritesheet + "/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png", q);
                    tmp.icon.setOrigin(tmp.icon.getLocalBounds().size.x / 2, tmp.icon.getLocalBounds().size.y / 2);

                    break;
                }
            }

            auto strRepo = CoreManager::getInstance().getStrRepo();
            std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

            tmp.num.defaultStyleSetFont(font);
            tmp.num.defaultStyleSetCharSize(30);
            tmp.num.defaultStyleSetColor(sf::Color::White);
            tmp.num.reset();
            tmp.num.append(Func::num_padding(tmp.amount, 3));

            tmp.num_shadow.defaultStyleSetFont(font);
            tmp.num_shadow.defaultStyleSetCharSize(30);
            tmp.num_shadow.defaultStyleSetColor(sf::Color(136, 136, 136, 255));
            tmp.num_shadow.reset();
            tmp.num_shadow.append(Func::num_padding(tmp.amount, 3));

            inventory_boxes.push_back(tmp);
        }
    }

    std::sort(inventory_boxes.begin(), inventory_boxes.end(),
              [](const InvBox& a, const InvBox& b) {
                  return a.data->order_id < b.data->order_id;
              });

    if ((old_invboxes.size() > 0) || (!save_loaded)) ///if the invboxes are empty, don't highlight, unless it's a fresh save
    {
        ///Now check for new items
        for (int a = 0; a < inventory_boxes.size(); a++)
        {
            ///get invbox item from current table
            bool found = false;
            bool highlight = false;

            for (int b = 0; b < old_invboxes.size(); b++)
            {
                ///and compare it to every invbox item in the old table
                if (inventory_boxes[a].data->order_id == old_invboxes[b].data->order_id)
                {
                    found = true;

                    if (inventory_boxes[a].amount > old_invboxes[b].amount)
                    {
                        highlight = true;
                    }

                    if (old_invboxes[b].highlight)
                        highlight = false;
                }
            }

            SPDLOG_TRACE("Check invbox {} found {} highlight {}", a, found, highlight);

            if (!found)
                highlight = true;

            if (highlight)
            {
                inventory_boxes[a].r_highlight.setSize(sf::Vector2f(104.0 * res_ratio_x, 77.0 * res_ratio_y));
                inventory_boxes[a].r_highlight.setFillColor(sf::Color::White);

                inventory_boxes[a].highlight = true;
            }
        }
    }

    updateAltarDescriptions();
}

void AltarMenu::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    sf::View lastView = window->getView();
    window->setView(window->getDefaultView());

    if (true)
    {
        InputController* inputCtrl = CoreManager::getInstance().getInputController();
        float fps = CoreManager::getInstance().getCore()->getFPS();

        highlight_x += 7.0 / fps;

        // TO-DO: control tips for new system
        /* ctrlTips.x = 0;
        ctrlTips.y = (720 - ctrlTips.ySize);
        ctrlTips.draw(window); */

        altar_main.setOrigin(altar_main.getLocalBounds().size.x / 2, altar_main.getLocalBounds().size.y / 2);
        altar_main.setPosition(1020, 1080);

        altar_main.draw();

        for (int i = 0; i < 24; i++)
        {
            if (grid_offset_y * 4 + i < inventory_boxes.size())
            {
                int curItem = grid_offset_y * 4 + i;

                int grid_x = i % 4;
                int grid_y = floor(i / 4);

                float xpos = 72 + (grid_x * 118);
                float ypos = 64 + (grid_y * 88);

                inventory_boxes[curItem].r_outer.setPosition(sf::Vector2f((40 + xpos) * res_ratio_x, (39 + ypos) * res_ratio_y));
                inventory_boxes[curItem].r_inner.setPosition(sf::Vector2f((40 + xpos + 2.5) * res_ratio_x, (39 + ypos + 2.5) * res_ratio_y));
                window->draw(inventory_boxes[curItem].r_outer);
                window->draw(inventory_boxes[curItem].r_inner);

                //inventory_boxes[i].num.setOrigin(inventory_boxes[i].num.getLocalBounds().size.x,inventory_boxes[i].num.getLocalBounds().size.y);
                //inventory_boxes[i].num_shadow.setOrigin(inventory_boxes[i].num_shadow.getLocalBounds().size.x,inventory_boxes[i].num_shadow.getLocalBounds().size.y);

                if ((inventory_boxes[curItem].data->item_category == "key_items") || (inventory_boxes[curItem].data->item_category == "materials")) ///Bound to break
                    inventory_boxes[curItem].icon.setScale(0.64, 0.64);

                inventory_boxes[curItem].icon.setPosition((40 + xpos + 36 + 2.5)*3, (39 + ypos + 36 + 2.5)*3);
                inventory_boxes[curItem].icon.draw();

                inventory_boxes[curItem].num.setGlobalPosition((40 + xpos + 51 - 1)*3, (39 + ypos + 45 - 2)*3);
                inventory_boxes[curItem].num_shadow.setGlobalPosition((40 + xpos + 51) * 3, (39 + ypos + 45) * 3);

                inventory_boxes[curItem].num_shadow.draw();
                inventory_boxes[curItem].num.draw();

                if (inventory_boxes[curItem].highlight)
                {
                    inventory_boxes[curItem].r_highlight.setPosition({(40 + xpos) * res_ratio_x, (39 + ypos) * res_ratio_y});
                    inventory_boxes[curItem].r_highlight.setFillColor(sf::Color(255, 255, 255, 64 + (sin(highlight_x) * 64)));
                    window->draw(inventory_boxes[curItem].r_highlight);
                }
            } else
            {
                InvBox tmp_inv;

                int grid_x = i % 4;
                int grid_y = floor(i / 4);

                float xpos = 72 + (grid_x * 118);
                float ypos = 64 + (grid_y * 88);

                tmp_inv.r_outer.setSize(sf::Vector2f(104.0 * res_ratio_x, 77.0 * res_ratio_y));
                tmp_inv.r_outer.setFillColor(sf::Color(102, 102, 102, 255));

                tmp_inv.r_inner.setSize(sf::Vector2f(72.0 * res_ratio_x, 72.0 * res_ratio_y));
                tmp_inv.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

                tmp_inv.r_outer.setPosition(sf::Vector2f((40 + xpos) * res_ratio_x, (39 + ypos) * res_ratio_y));
                tmp_inv.r_inner.setPosition(sf::Vector2f((40 + xpos + 2.5) * res_ratio_x, (39 + ypos + 2.5) * res_ratio_y));
                window->draw(tmp_inv.r_outer);
                window->draw(tmp_inv.r_inner);
            }
        }

        r_sel.setSize(sf::Vector2f(103.0 * res_ratio_x, 77.0 * res_ratio_y));
        r_sel.setFillColor(sf::Color::Transparent);
        r_sel.setOutlineThickness(3);
        r_sel.setOutlineColor(sf::Color(255, 0, 32, 255));
        r_sel.setPosition({(41 + 72 + (grid_sel_x * 118)) * res_ratio_x, (39 + 64 + (grid_sel_y * 88)) * res_ratio_y});

        window->draw(r_sel);

        rr_title.Create(366 * 3, 100 * 3, 20 * 3, window->getSize().x / float(3840));
        rr_title.x = 933 * 3;
        rr_title.y = 141 * 3;
        rr_title.setOrigin(sf::Vector2f((rr_title.width + 40) / 2, (rr_title.height + 40) / 2));

        rr_title_sh.Create(rr_title.width + 2, rr_title.height + 2, 20 * 3, window->getSize().x / float(3840), sf::Color(0, 0, 0, 96));
        rr_title_sh.x = rr_title.x - 1;
        rr_title_sh.y = rr_title.y - 1;
        rr_title_sh.setOrigin(sf::Vector2f((rr_title.width + 40) / 2, (rr_title.height + 40) / 2));

        rr_title_sh.Draw();
        rr_title.Draw();

        rr_desc.Create(440 * 3, 385 * 3, 20 * 3, window->getSize().x / float(3840));
        rr_desc.x = rr_title.x;
        rr_desc.y = 436 * 3;
        rr_desc.setOrigin(sf::Vector2f((rr_desc.width + 40) / 2, (rr_desc.height + 40) / 2));

        rr_desc_sh.Create(rr_desc.width + 2, rr_desc.height + 2, 20 * 3, window->getSize().x / float(3840), sf::Color(0, 0, 0, 96));
        rr_desc_sh.x = rr_desc.x - 1;
        rr_desc_sh.y = rr_desc.y - 1;
        rr_desc_sh.setOrigin(sf::Vector2f((rr_desc.width + 40) / 2, (rr_desc.height + 40) / 2));

        rr_desc_sh.Draw();
        rr_desc.Draw();

        altar_title.setGlobalOrigin(altar_title.getGlobalBounds().size.x / 2, altar_title.getGlobalBounds().size.y / 2);
        altar_kaching.setGlobalOrigin(altar_kaching.getGlobalBounds().size.x / 2, altar_kaching.getGlobalBounds().size.y / 2);

        altar_title.setGlobalPosition(933 * 3, 100 * 3);
        altar_kaching.setGlobalPosition(933 * 3, 170 * 3);

        altar_item_title.setGlobalPosition(933 * 3, 250 * 3);
        altar_item_category.setGlobalPosition(933 * 3, 280 * 3);
        altar_item_desc.setGlobalPosition(725 * 3, 330 * 3);

        altar_title.draw();
        altar_kaching.draw();

        altar_item_title.setGlobalOrigin(altar_item_title.getGlobalBounds().size.x / 2, altar_item_title.getGlobalBounds().size.y / 2);
        altar_item_category.setGlobalOrigin(altar_item_category.getGlobalBounds().size.x / 2, altar_item_category.getGlobalBounds().size.y / 2);
        altar_item_desc.setGlobalOrigin(0, 0);

        altar_item_title.draw();
        altar_item_category.draw();
        altar_item_desc.draw();

        if (inputCtrl->isKeyPressed(Input::Keys::LEFT))
        {
            grid_sel_x--;

            if (grid_sel_x < 0)
                grid_sel_x = 3;

            updateAltarDescriptions();
        }
        if (inputCtrl->isKeyPressed(Input::Keys::RIGHT))
        {
            grid_sel_x++;

            if (grid_sel_x > 3)
                grid_sel_x = 0;

            updateAltarDescriptions();
        }
        if (inputCtrl->isKeyPressed(Input::Keys::UP))
        {
            grid_sel_y--;

            if (grid_sel_y < 0)
            {
                if (grid_offset_y > 0)
                {
                    grid_offset_y--;
                    grid_sel_y = 0;
                } else
                {
                    grid_offset_y = ceil(inventory_boxes.size() / 4.0) - 6;

                    if (grid_offset_y < 0)
                        grid_offset_y = 0;

                    grid_sel_y = 5;
                }
            }

            updateAltarDescriptions();
        }
        if (inputCtrl->isKeyPressed(Input::Keys::DOWN))
        {
            grid_sel_y++;

            if (grid_sel_y > 5)
            {
                if (inventory_boxes.size() > (6 + grid_offset_y) * 4)
                {
                    grid_offset_y++;
                    grid_sel_y = 5;
                } else
                {
                    grid_sel_y = 0;
                    grid_offset_y = 0;
                }
            }

            updateAltarDescriptions();
        }
        if (inputCtrl->isKeyPressed(Input::Keys::CIRCLE))
        {
            StateManager::getInstance().setState(StateManager::PATAPOLIS);
        }
    }

    window->setView(lastView);
}

AltarMenu::~AltarMenu()
{
    //dtor
}
