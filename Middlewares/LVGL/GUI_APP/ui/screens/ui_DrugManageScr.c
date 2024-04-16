﻿// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: 0_SmartPillBox

#include "../ui.h"

void ui_DrugManageScr_screen_init(void)
{
    ui_DrugManageScr = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_DrugManageScr, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel6 = lv_obj_create(ui_DrugManageScr);
    lv_obj_set_width(ui_Panel6, 820);
    lv_obj_set_height(ui_Panel6, 60);
    lv_obj_set_x(ui_Panel6, -10);
    lv_obj_set_y(ui_Panel6, 0);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton1 = lv_imgbtn_create(ui_Panel6);
    lv_imgbtn_set_src(ui_ImgButton1, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_back_png, NULL);
    lv_obj_set_height(ui_ImgButton1, 64);
    lv_obj_set_width(ui_ImgButton1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton1, -10);
    lv_obj_set_y(ui_ImgButton1, 5);
    lv_obj_set_align(ui_ImgButton1, LV_ALIGN_LEFT_MID);

    ui_Label4 = lv_label_create(ui_Panel6);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER); 
    lv_label_set_text(ui_Label4, "药物管理");
    lv_obj_set_style_text_font(ui_Label4, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_STATE_DEFAULT);
	
	ui_DrugTable = lv_table_create(ui_DrugManageScr);
	lv_table_set_row_cnt(ui_DrugTable, 1);
    lv_table_set_col_cnt(ui_DrugTable, 5);
    lv_obj_set_size(ui_DrugTable, 800, 420);
	lv_obj_set_pos(ui_DrugTable, 0, 60);
	lv_obj_set_style_text_font(ui_DrugTable, &ui_font_SourceHanSansCN30, LV_PART_MAIN | LV_PART_ITEMS);
	lv_obj_set_style_border_color(ui_DrugTable, lv_color_hex(0x000000), LV_PART_MAIN | LV_PART_ITEMS);
    lv_obj_set_style_border_opa(ui_DrugTable, 255, LV_PART_MAIN | LV_PART_ITEMS);
	lv_table_set_col_width(ui_DrugTable, 0, 100);
	lv_table_set_cell_value(ui_DrugTable, 0, 0, "序号");
    lv_table_set_col_width(ui_DrugTable, 1, 220);
	lv_table_set_cell_value(ui_DrugTable, 0, 1, "药物名");
    lv_table_set_col_width(ui_DrugTable, 2, 110);
	lv_table_set_cell_value(ui_DrugTable, 0, 2, "数量");
    lv_table_set_col_width(ui_DrugTable, 3, 180);
	lv_table_set_cell_value(ui_DrugTable, 0, 3, "药仓编号");
    lv_table_set_col_width(ui_DrugTable, 4, 190);
	lv_table_set_cell_value(ui_DrugTable, 0, 4, "服用剂量");

    lv_obj_add_event_cb(ui_ImgButton1, ui_event_ImgButton1, LV_EVENT_ALL, NULL);

}
