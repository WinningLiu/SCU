from PIL import Image, ImageDraw
import csv

img = Image.new('RGB', (600, 600))
pixels = img.load()

vertices = []

# Draw pixel given x and y
def draw_pixel(x, y):
    pixels[x, y] = (255, 0, 0)

with open('face-vertices.data', 'r') as csvfile:
    csvreader = csv.reader(csvfile, delimiter = ',')

    for row in csvreader:
        vertices.append(row)

    vertices_prime = []

    # d = 1
    for i in vertices:
        # Scale the image and shift it to the middle of the screen
        i[0] = round(float(i[0])/(1-float(i[2])) * -400) + 300 # x` = x/(1-(z/d)) 
        i[1] = round(float(i[1])/(1-float(i[2])) * -400) + 300 # y` = y/(1-(z/d))
        i[2] = 0 # z` = 0

        vertices_prime.append(i)

        # Draw pixel given x` and y`
        draw_pixel(i[0], i[1])

    img.save('face-vertices.png')

with open('face-index.txt', 'r') as csvfile:
    csvreader = csv.reader(csvfile, delimiter = ',')

    draw = ImageDraw.Draw(img)

    # i = the index in verticies prime
    for i in csvreader:
        # Set vertices to be drawn for a triangle
        v0 = vertices_prime[int(i[0])]
        v1 = vertices_prime[int(i[1])]
        v2 = vertices_prime[int(i[2])]

        draw.line((v0[0], v0[1], v1[0], v1[1]), fill = 255) #Line from v0 to v1
        draw.line((v1[0], v1[1], v2[0], v2[1]), fill = 255) #Line from v1 to v2
        draw.line((v2[0], v2[1], v0[0], v0[1]), fill = 255) #Line from v2 to v0

    img.save('face-triangles.png')