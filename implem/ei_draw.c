#include "ei_draw.h"


//Draw a line using Bresenham's algo (vue en cours) (to be used only in this file)
void static ei_draw_line(ei_surface_t surface, ei_point_t p1, ei_point_t p2, ei_color_t color) {
        int x0 = p1.x, y0 = p1.y;
        int x1 = p2.x, y1 = p2.y;

        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        ei_size_t size = hw_surface_get_size(surface);

        while (true) {
                // Set pixel at (x0, y0)
                if (x0 >= 0 && x0 < size.width && y0 >= 0 && y0 < size.height) {
                        uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
                        pixel_ptr += y0 * size.width + x0;
                        *pixel_ptr = (color.alpha << 24) | (color.red << 16) | (color.green << 8) | color.blue;
                }

                if (x0 == x1 && y0 == y1) break;
                e2 = 2 * err;
                if (e2 >= dy) { err += dy; x0 += sx; }
                if (e2 <= dx) { err += dx; y0 += sy; }
        }
};

//Remarques
// 1. Clipper case to be implemented
// 2. If there's only one points, one simple dot ?
// 3. Alpha channel to manage ?
void	ei_draw_polyline	(ei_surface_t		surface,
                                     ei_point_t*		point_array,
                                     size_t			point_array_size,
                                     ei_color_t		color,
                                     const ei_rect_t*	clipper) {

        for (size_t i = 0; i < point_array_size - 1; i++) {
                ei_point_t start_point = point_array[i];
                ei_point_t end_point = point_array[i + 1];

                ei_draw_line(surface, start_point, end_point, color);
        }
}


void	ei_draw_polygon		(ei_surface_t		surface,
                                            ei_point_t*		point_array,
                                            size_t			point_array_size,
                                            ei_color_t		color,
                                            const ei_rect_t*	clipper);

void	ei_draw_text		(ei_surface_t		surface,
                                         const ei_point_t*	where,
                                         ei_const_string_t	text,
                                         ei_font_t		font,
                                         ei_color_t		color,
                                         const ei_rect_t*	clipper);

void	ei_fill			(ei_surface_t		surface,
                                            const ei_color_t*	color,
                                            const ei_rect_t*	clipper){

        uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        ei_size_t size = hw_surface_get_size(surface);
        if(clipper == NULL){
                for (int i = 0; i < size.width * size.height; i++) { // Iterate over each pixel in the buffer
                        *pixel_ptr++ = (color->alpha << 24) | // Alpha component
                                       (color->red << 16) |   // Red component
                                       (color->green << 8) |  // Green component
                                       color->blue;           // Blue component
                }
        } else {
                for (int i = 0; i < clipper->top_left.y * size.width; i++) {
                        pixel_ptr++;
                }
                for (int j = 0; j < clipper->size.height; j++) {
                        pixel_ptr += clipper->top_left.x;
                        for (int k = 0; k < clipper->size.width; k++) {
                                *pixel_ptr++= (color->alpha << 24) | // Alpha component
                                              (color->red << 16) |   // Red component
                                              (color->green << 8) |  // Green component
                                              color->blue;           // Blue component
                        }
                        pixel_ptr += size.width - (clipper->top_left.x + clipper->size.width);

                }
        }


}


int	ei_copy_surface		(ei_surface_t		destination,
                                           const ei_rect_t*	dst_rect,
                                           ei_surface_t		source,
                                           const ei_rect_t*	src_rect,
                                           bool			alpha);




