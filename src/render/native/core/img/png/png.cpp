#include "./png.hpp"

bool ispng (uint16_t* buf) {
    uint16_t GIF[4] = {0x5089, 0x474E, 0x0A0D, 0x0A1A};
    if (buf[0] == GIF[0] && buf[1] == GIF[1] && buf[2] == GIF[2] && buf[3] == GIF[3]) {
        return true;
    }
    return false;
};

void convert_color_depth(uint8_t * img, uint32_t px_cnt)
{
    /* In LVGL v9 with LV_COLOR_DEPTH 32, the native format is XRGB8888/ARGB8888.
     * lodepng decodes to RGBA, so we need to convert RGBA -> BGRA (ARGB8888 in little-endian).
     * Swap red and blue channels. */
    lv_color32_t * img_argb = (lv_color32_t*)img;
    uint32_t i;
    for(i = 0; i < px_cnt; i++) {
        uint8_t tmp = img_argb[i].red;
        img_argb[i].red = img_argb[i].blue;
        img_argb[i].blue = tmp;
    }
}

static lv_result_t decoder_info(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc, lv_image_header_t * header) {
    LV_UNUSED(decoder);
    const lv_img_dsc_t_1* img_dsc = (const lv_img_dsc_t_1*)dsc->src;
    if(img_dsc->type == IMAGE_TYPE_PNG) {
        header->magic = LV_IMAGE_HEADER_MAGIC;
        header->cf = img_dsc->header.cf;
        header->w = img_dsc->header.w;
        header->h = img_dsc->header.h;
        header->stride = img_dsc->header.stride;
        header->flags = 0;
        header->reserved_2 = 0;
        return LV_RESULT_OK;
    }

    return LV_RESULT_INVALID;
};

void lv_png_init(void)
{
    lv_image_decoder_t * dec = lv_image_decoder_create();
    lv_image_decoder_set_info_cb(dec, decoder_info);
};
