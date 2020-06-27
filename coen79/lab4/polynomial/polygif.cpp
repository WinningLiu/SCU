// FILE: polygif.cxx
// This file implements one non-member function (make_gif) for the polynomial
// class. The implementation does not use compression and other features of
// GIF files.

#include "poly.h"        // Provides polynomial class definition
#include "poly.cpp"
#include <fstream>        // Provides istream, ostream and ofstream types
#include <cassert>        // Provides assert
using namespace std;      // All the above items are in the std namespace

namespace coen79_lab4
{
    //   void make_gif(
    //     const polynomial& p,
    //     const char filename[ ],
    //     double low_x,
    //     double high_x,
    //     double low_y,
    //     double high_y
    //     )
    //     PRECONDITION: filename is a legal filename for a gif file.
    //     Also (low_x < high_x) && (low_y < high_y).
    //     POSTCONDITION: A gif file has been written to the specified filename
    //     with a graphical representation of the polynomial in the specified
    //     ranges (low_x...high_x and low_y...high_y).
    
    void write_gif_header128(
                             ostream& out,
                             int width,
                             int height,
                             const char color_table[128][3]
                             )
    {
        // GIF signature and screen descriptor:
        out.write("GIF87a", 6);         // GIF signature
        out.put(width & 0xFF);          // Low byte of screen width
        out.put((width >> 8) & 0xFF);   // High byte of screen width
        out.put(height & 0xFF);         // Low byte of screen width
        out.put((height >> 8) & 0xFF);  // High byte of screen width
        out.put(0xE6);                  // 7-bits per pixel
        out.put(0);                     // Background color
        out.put(0);                     // Zero marks end of screen descriptor
        
        // Color table:
        out.write(color_table[0], 128*3);
        
        // Image descriptior:
        out.put(',');                   // Image separator character
        out.write("\0\0\0\0", 4);       // Image starts at 0,0
        out.put(width & 0xFF);          // Low byte of image width
        out.put((width >> 8) & 0xFF);   // High byte of image width
        out.put(height & 0xFF);         // Low byte of image width
        out.put((height >> 8) & 0xFF);  // High byte of image width
        out.put('\0');                  // Non-interleaved image
        
        // Code size (with 128 colors, pixels are 7 bits each).
        out.put('\7'); // Code size (7-bit data for each pixel).
    }
    
    void make_gif(
                  const polynomial& p,
                  const char filename[ ],
                  double low_x,
                  double high_x,
                  double low_y,
                  double high_y
                  )
    {
        enum color { BLACK = 0, DARKGRAY, LIGHTGRAY, RED, GREEN };
        const char COLOR_TABLE[128][3] = {
            {'\000', '\000', '\000' },  // BLACK
            {'\100', '\100', '\100' },  // DARKGRAY
            {'\300', '\300', '\300' },  // LIGHTGRAY
            {'\250', '\000', '\000' },  // RED
            {'\000', '\250', '\000' }   // GREEN
        };
        
        const int SIZE  = 1000; // Gif map width and height
        
        double delta_x = (high_x - low_x) / (SIZE - 1);
        double delta_y   = (high_y - low_y) / (SIZE - 1);
        bool vertical_axis, horizontal_axis;
        int row, column;
        double current_x, current_y;
        double nearness;
        double value[SIZE];
        char color;
        
        // Check that the file opened okay and write the front
        // information to the file.
        ofstream gif(filename, ios::out|ios::binary);
        assert(gif);
        
        write_gif_header128(gif, SIZE, SIZE, COLOR_TABLE);
        
        // Compute the value of the function at each column
        for (column = 0; column < SIZE; column++)
            value[column] = p.eval(low_x + column * delta_x);
        
        // Write the pixel data for the picture
        for (row = SIZE-1; row >= 0; --row)
        {
            current_y = low_y + row * delta_y;
            horizontal_axis = (delta_y >= current_y) && (current_y > -delta_y);
            
            for (column = 0; column < SIZE; ++column)
            {
                if ((column % 5) == 0)
                {
                    gif.put('\6');
                    gif.write("\200", 1); // Start code for 8-bit codes
                }
                
                current_x = low_x + column * delta_x;
                vertical_axis = (delta_x >= current_x) && (current_x > -delta_x);
                nearness = current_y - value[column];
                
                if ((delta_y >= nearness) && (nearness >= -delta_y))
                    color = BLACK;
                else if (vertical_axis || horizontal_axis)
                    color = DARKGRAY;
                else if ((current_y < value[column]) && (current_y >= 0))
                    color = GREEN;
                else if ((current_y > value[column]) && (current_y < 0))
                    color = RED;
                else
                    color = LIGHTGRAY;
                
                gif.put(color);
            }
        }
        
        // Write the GIF ending and close the file
        gif.put('\1');      // Block byte count
        gif.put('\201');    // End code for 8-bit codes
        gif.put('\0');      // Data Terminator
        gif.put('\73');     // GIF File Terminator
        gif.close( );
    }
    
}
