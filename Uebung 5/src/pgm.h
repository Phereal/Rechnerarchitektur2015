

#ifndef RAUES_PGM_HPP
#define RAUES_PGM_HPP

#include <vector>
#include <string>






/**
 * @brief Typedef for a pgm image entry
 */

using pgm_entry = unsigned char;


/**
 * @brief Typedef for a line in a pgm image
 */
using pgm_line = std::vector<pgm_entry>;

/**
 * @brief Typedef for pgm images
 *
 * This is just here to avoid typing too much.
 *
 * @note Data of type pgm_image implicitly contains the information about height, widths
 * and color depth. For convenience, these values are stored in separate variables when
 * reading a pgm from a file.
 *
 * @note Height, width and color depth are required to save the pgm image!
 */
using pgm_image = std::vector<pgm_line>;


/**
 * @brief Writes PGM image to file
 *
 * @param filename The file to write to
 * @param width Width of the image
 * @param height Height of the image
 * @param image The actual image data
 * @param color_depth The color depth of the image. If not specified, 255 is assumed
 */
void write_pgm(std::string filename, int width, int height, const pgm_image& image,int color_depth=255);


/**
 * @brief Reads a PGM image from a file
 *
 * This method uses call-by-solution on the variables width, height, depth and image. This means
 * that the function modifies its parameters writing the image data (and metadata) into them
 *
 * @param filename The file to read from
 * @param height Height of the image
 * @param width The width of the image
 * @param color_depth The color depth of the image
 * @param image The actual imagae data
 */
void read_pgm(std::string filename, int& width, int& height, int& color_depth, pgm_image& image);
#endif //RAUES_PGM_HPP
