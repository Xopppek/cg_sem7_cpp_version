import imageio.v2 as imageio #Import version 2 explicitly
import os
import glob

def create_gif(image_dir, output_filename, fps=30):
    images = []
    for filename in sorted(glob.glob(os.path.join(image_dir, "*.png"))):
        images.append(imageio.imread(filename)) #Now uses imageio.v2.imread
    imageio.mimsave(output_filename, images, fps=fps, loop=0)

if __name__ == "__main__":
    image_dir = "images/perspective"  # Directory with your images
    output_filename = "images/perspective.gif"
    create_gif(image_dir, output_filename)
    print(f"GIF created: {output_filename}")
    image_dir = "images/parallel"  # Directory with your images
    output_filename = "images/parallel.gif"
    create_gif(image_dir, output_filename)
    print(f"GIF created: {output_filename}")