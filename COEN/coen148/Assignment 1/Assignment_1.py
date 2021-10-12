from PIL import Image

row = 400
col = 400
img = Image.new('RGB', (row, col)) 
pixels = img.load()
color = (255, 0, 0)
blurred = (128 , 0, 0)

#Question 1
def circle_point(pixels, x, y):
    #Draw 7 symmetric pixels, given one pixel
    #Shift to the middle of the image
    x += 200
    y += 200

    pixels[x, y] = color
    pixels[-x, y] = color
    pixels[-x, -y] = color
    pixels[x, -y] = color
    pixels[y, x] = color
    pixels[-y, x] = color
    pixels[-y, -x] = color
    pixels[y, -x] = color

def draw_circle(pixels, radius):
    #Initial values
    d = (5/4) - radius
    x = 0
    y = radius

    #Fill in initial point
    circle_point(pixels, x, y)

    #Midpoint algorithm
    while x < y:
        if d < 0:
            d += 2*x + 3
        else:
            d += (2*x) - (2*y) - 5
            y -= 1
        x += 1
        circle_point(pixels, x, y) #once the pixel is decided, fill in the point

#Question 2
def fill_row(pixels, first, last, j):
    #Line contains no pixels of the desired color, so no action is needed
    if first == -1 or last == -1:
        return
    #Fill in all pixels between first and last pixels needed color
    for i in range(first, last + 1):
        pixels[i, j] = color

def scan_line(pixels):
    for j in range(0, col):
        #Set initial values to -1
        first = -1
        last = -1
        for i in range(0, row):
            #Record first and last pixels where it is equal to the color
            if pixels[i, j] == color and first == -1:
                first = i
            if pixels[i, j] == color:
                last = i
        fill_row(pixels, first, last, j)

def circle_point_blurred(pixels, x, y):
    #Draw 7 symmetric pixels, given one pixel
    #Shift to the middle of the image
    x += 200
    y += 200

    pixels[x, y] = blurred
    pixels[-x, y] = blurred
    pixels[-x, -y] = blurred
    pixels[x, -y] = blurred
    pixels[y, x] = blurred
    pixels[-y, x] = blurred
    pixels[-y, -x] = blurred
    pixels[y, -x] = blurred

def anti_alias(pixels, radius):
    #Initial values
    d = (5/4) - radius 
    x = 0
    y = radius

    #Fill in initial point
    circle_point_blurred(pixels, x, y)

    #Midpoint algorithm
    while x < y:
        if d < 0:
            d += 2*x + 3
        else:
            d += (2*x) - (2*y) - 5
            y -= 1
        x += 1
        circle_point_blurred(pixels, x, y) #once the pixel is decided, fill in the point

draw_circle(pixels, 100)
img.save('q1.png')
#Draw a blurred circle with 1 larger and smaller radius than the original circle
anti_alias(pixels, 101)
anti_alias(pixels, 99)
img.save('q3.png')
scan_line(pixels)
img.save('q2.png')
