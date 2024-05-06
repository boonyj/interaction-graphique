#include "ei_draw.h"


void	ei_draw_polyline	(ei_surface_t		surface,
                                     ei_point_t*		point_array,
                                     size_t			point_array_size,
                                     ei_color_t		color,
                                     const ei_rect_t*	clipper);


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




