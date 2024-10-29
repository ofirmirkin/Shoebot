import albumentations as A
import cv2
import numpy as np
import os

# Define a custom augmentation pipeline
transform = A.Compose(
    [
        A.Rotate(limit=10, p=0.3),  # Slight rotation
        A.ShiftScaleRotate(
            shift_limit=0.0625, scale_limit=0.05, rotate_limit=0, p=0.7
        ),  # Slight shift and scale
        A.OneOf(
            [
                A.MotionBlur(blur_limit=(3, 5), p=0.5),
                A.GaussianBlur(blur_limit=(3, 5), p=0.5),
            ],
            p=0.5,
        ),  # Motion blur or Gaussian blur
        A.RandomBrightnessContrast(
            brightness_limit=0.2, contrast_limit=0.2, p=0.8
        ),  # Stronger brightness/contrast adjustments for darker/lighter effects
        A.CLAHE(
            clip_limit=2.0, tile_grid_size=(8, 8), p=0.3
        ),  # Adaptive contrast to handle extreme lighting
        A.HueSaturationValue(
            hue_shift_limit=10, sat_shift_limit=15, val_shift_limit=20, p=0.5
        ),  # Increased value shift to handle lighting
        A.RandomShadow(
            shadow_roi=(0, 0.5, 1, 1),
            num_shadows_lower=1,
            num_shadows_upper=2,
            shadow_dimension=5,
            p=0.1,
        ),  # Random shadow
        A.Perspective(scale=(0.02, 0.05), p=0.5),  # Perspective transform
        # Horizontal flip could be interesting
    ]
)


def augment_images(input_dir: str, output_dir: str, augmentation_factor: int = 2):
    """Run image augmentation on a directory of images and save the results to a new directory.

    Args:
        augmentation_factor (int, optional): The number of augmented images to generate for each input image. Defaults to 2.
    """
    # Iterate over each class folder in the input directory
    for class_folder in os.listdir(input_dir):
        class_input_path = os.path.join(input_dir, class_folder)
        if not os.path.isdir(class_input_path):
            continue  # Skip non-directory files

        # Create corresponding output directory for the class
        class_output_path = os.path.join(output_dir, class_folder)
        os.makedirs(class_output_path, exist_ok=True)

        # Iterate over each image in the class folder
        for filename in os.listdir(class_input_path):
            if filename.lower().endswith((".jpg", ".png", ".jpeg")):
                image_path = os.path.join(class_input_path, filename)
                image = cv2.imread(image_path)
                if image is None:
                    print(
                        f"Warning: Could not read image {filename} in {class_folder}. Skipping."
                    )
                    continue
                image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

                # Save the original image
                cv2.imwrite(
                    os.path.join(class_output_path, filename),
                    cv2.cvtColor(image, cv2.COLOR_RGB2BGR),
                )

                # Generate augmented images
                for i in range(augmentation_factor - 1):
                    augmented = transform(image=image)["image"]
                    aug_filename = f"{os.path.splitext(filename)[0]}_aug_{i}{os.path.splitext(filename)[1]}"
                    cv2.imwrite(
                        os.path.join(class_output_path, aug_filename),
                        cv2.cvtColor(augmented, cv2.COLOR_RGB2BGR),
                    )


# Example usage
augment_images(
    "shoesSet",
    "augmentedShoesSet",
    augmentation_factor=3,
)
