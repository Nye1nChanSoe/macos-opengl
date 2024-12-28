from PIL import Image
import os
import sys

"""
Usage: python extract_cubemap.py <image_name>
"""

"This script extracts the faces of a cubemap image and saves them as separate images."
"Only supports horizontal cross layout cubemaps."


CUBEMAP_PATH = "../assets/cubemaps/"
CUBEMAP_SIZE_4K = 4096
FACE_SIZE_4K = 1024
CUBEMAP_SIZE_8K = 8192
FACE_SIZE_8K = 2048


def load_horizontal_cubemap(cube_map_path: str) -> Image:
    return Image.open(cube_map_path)


def extract_faces_from_cubemap(cubemap: Image.Image, face_size: int) -> dict:
    return {
        "py": cubemap.crop((face_size, 0, 2 * face_size, face_size)),          # +Y (Top)
        "nx": cubemap.crop((0, face_size, face_size, 2 * face_size)),         # -X (Left)
        "pz": cubemap.crop((face_size, face_size, 2 * face_size, 2 * face_size)), # +Z (Front)
        "px": cubemap.crop((2 * face_size, face_size, 3 * face_size, 2 * face_size)), # +X (Right)
        "nz": cubemap.crop((3 * face_size, face_size, 4 * face_size, 2 * face_size)), # -Z (Back)
        "ny": cubemap.crop((face_size, 2 * face_size, 2 * face_size, 3 * face_size))  # -Y (Bottom)
    }


def save_faces_to_images(faces: dict, folder: str):
    output_path = os.path.join("../assets/cubemaps/faces", folder)
    create_folder_if_not_exists(output_path)

    for face_name, face_image in faces.items():
        face_path = os.path.join(output_path, f"{face_name}.png")
        face_image.save(face_path)
        print(f"Face '{face_name}' saved at: {face_path}")


def extract_name_from_path(path: str) -> str:
    return os.path.splitext(os.path.basename(path))[0]


def create_folder_if_not_exists(folder_path: str):
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)


def process_cubemap_image(cubemap_path: str):
    cubemap = load_horizontal_cubemap(cubemap_path)

    print(f"Cubemap Loaded: {cubemap_path}")
    print(f"Cubemap Resolution: {cubemap.width}x{cubemap.height}")


    # Determine face size dynamically based on the cubemap dimensions
    if cubemap.width == CUBEMAP_SIZE_8K:
        face_size = FACE_SIZE_8K
    elif cubemap.width == CUBEMAP_SIZE_4K:
        face_size = FACE_SIZE_4K
    else:
        raise ValueError("Invalid cubemap size. Must be 4k or 8k.")

    # Extract faces
    faces = extract_faces_from_cubemap(cubemap=cubemap, face_size=face_size)

    # save images
    folder_name = extract_name_from_path(cubemap_path)
    save_faces_to_images(faces=faces, folder=folder_name)



if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python extract_cubemap.py <image_name>")
        sys.exit(1)

    image_name = sys.argv[1]
    cubemap_full_path = os.path.join(CUBEMAP_PATH, image_name)

    process_cubemap_image(cubemap_full_path)