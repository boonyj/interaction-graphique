#include "ei_draw.h"
#include "ei_implementation.h"

void	ei_fill			(ei_surface_t		surface,
                                            const ei_color_t*	color,
                                            const ei_rect_t*	clipper){

        uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        ei_size_t size = hw_surface_get_size(surface);
        if(clipper == NULL){
                for (int i = 0; i < size.width * size.height; i++) { // Iterate over each pixel in the buffer
                        *pixel_ptr++ = ei_impl_map_rgba(surface, *color);
                }
        } else {
                for (int i = 0; i < clipper->top_left.y * size.width; i++) {
                        pixel_ptr++;
                }
                for (int j = 0; j < clipper->size.height; j++) {
                        pixel_ptr += clipper->top_left.x;
                        for (int k = 0; k < clipper->size.width; k++) {
                                *pixel_ptr++= ei_impl_map_rgba(surface, *color);
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




