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

void	ei_draw_text		(ei_surface_t		surface,
                                         const ei_point_t*	where,
                                         ei_const_string_t	text,
                                         ei_font_t		font,
                                         ei_color_t		color,
                                         const ei_rect_t*	clipper){


        ei_surface_t text_surface = hw_text_create_surface(text, font, color);
        ei_rect_t text_surface_rect = hw_surface_get_rect(surface);
        ei_rect_t dst_rect = hw_surface_get_rect(surface);
        dst_rect.top_left.x = where->x;
        dst_rect.top_left.y = where->y;
        ei_copy_surface(surface, &dst_rect, text_surface, &text_surface_rect, true);
}

int	ei_copy_surface		(ei_surface_t		destination,
                                           const ei_rect_t*	dst_rect,
                                           ei_surface_t		source,
                                           const ei_rect_t*	src_rect,
                                           bool			alpha){
        hw_surface_lock(destination);
        hw_surface_lock(source);

        ei_size_t dest_size = hw_surface_get_size(destination);
        ei_size_t source_size = hw_surface_get_size(source);

        // If destination rectangle is NULL, use entire destination surface
        ei_rect_t actual_dst_rect;
        if (dst_rect == NULL) {
                actual_dst_rect.top_left.x = 0;
                actual_dst_rect.top_left.y = 0;
                actual_dst_rect.size = dest_size;
        } else {
                actual_dst_rect = *dst_rect;
        }

        // If source rectangle is NULL, use entire source surface
        ei_rect_t actual_src_rect;
        if (src_rect == NULL) {
                actual_src_rect.top_left.x = 0;
                actual_src_rect.top_left.y = 0;
                actual_src_rect.size = source_size;
        } else {
                actual_src_rect = *src_rect;
        }

        // Check if source and destination rectangles have the same size
        if (actual_dst_rect.size.width != actual_src_rect.size.width ||
            actual_dst_rect.size.height != actual_src_rect.size.height) {
                // Unlock surfaces before returning failure
                hw_surface_unlock(destination);
                hw_surface_unlock(source);
                return 1; // Different sizes, return failure
        }

        for (int y = 0; y < actual_src_rect.size.height; y++) {
                for (int x = 0; x < actual_src_rect.size.width; x++) {
                        // Calculate coordinates in source and destination surfaces
                        int src_x = actual_src_rect.top_left.x + x;
                        int src_y = actual_src_rect.top_left.y + y;
                        int dest_x = actual_dst_rect.top_left.x + x;
                        int dest_y = actual_dst_rect.top_left.y + y;

                        // Check if within destination bounds
                        if (dest_x >= 0 && dest_x < dest_size.width &&
                            dest_y >= 0 && dest_y < dest_size.height) {
                                // Check if within source bounds
                                if (src_x >= 0 && src_x < source_size.width &&
                                    src_y >= 0 && src_y < source_size.height) {
                                        // Copy pixel from source to destination
                                        uint8_t* source_pixel = hw_surface_get_buffer(source) + (src_y * source_size.width + src_x) * 4; // Assuming 32-bit RGBA
                                        uint8_t* dest_pixel = hw_surface_get_buffer(destination) + (dest_y * dest_size.width + dest_x) * 4; // Assuming 32-bit RGBA
                                        if (alpha) {
                                                // Alpha blending
                                                float alpha_value = source_pixel[3] / 255.0f;
                                                for (int i = 0; i < 3; i++) {
                                                        dest_pixel[i] = (uint8_t)((1 - alpha_value) * dest_pixel[i] + alpha_value * source_pixel[i]);
                                                }
                                                dest_pixel[3] = 255; // Set destination alpha to opaque
                                        } else {
                                                // Exact copy including alpha channel
                                                for (int i = 0; i < 4; i++) {
                                                        dest_pixel[i] = source_pixel[i];
                                                }
                                        }
                                }
                        }
                }
        }

        // Unlock surfaces
        hw_surface_unlock(destination);
        hw_surface_unlock(source);

        return 0; // Success

}




