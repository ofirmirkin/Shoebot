import os
from datetime import datetime

import cv2
import numpy as np
import predict
from flask import Flask, jsonify, request

# Define the Flask app
app = Flask(__name__)

# Path to save the uploaded images
UPLOAD_FOLDER = "uploads/"
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)


# Health check route
@app.route("/health", methods=["GET"])
def health_check():
    return jsonify({"status": "healthy"}), 200


# Route to handle image upload
@app.route("/upload", methods=["POST"])
def upload_image():
    start_time = datetime.now()  # Record the start time
    print(f"[DEBUG] Request started at: {start_time}")

    # Check if an image was sent with the request
    if "image" not in request.files:
        return (
            jsonify({"status": "error", "message": "No image file in the request"}),
            400,
        )

    # Get the image from the request
    file = request.files["image"]

    # Check if the image is a valid file
    if file.filename == "":
        return jsonify({"status": "error", "message": "No selected file"}), 400

    try:
        # Read the uploaded image file into memory
        npimg = np.frombuffer(file.read(), np.uint8)
        image = cv2.imdecode(npimg, cv2.IMREAD_COLOR)

        if image is None:
            raise ValueError("Failed to decode image")

        # Flip the image horizontally and vertically before saving
        flipped_image = cv2.flip(image, -1)

        # Generate a unique filename based on the current time
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"image_{timestamp}.jpg"
        # filename = f"image_1.jpg"
        file_path = os.path.join(UPLOAD_FOLDER, filename)

        # Save the flipped image
        cv2.imwrite(file_path, flipped_image)

        # Perform inference on the uploaded image
        model = predict.Predictor(model_path="model.pth")
        prediction = model.predict_image(file_path)

        end_time = datetime.now()  # Record the end time
        print(f"[DEBUG] Request ended at: {end_time}")
        print(f"[DEBUG] Total processing time: {end_time - start_time}")

        return (
            jsonify(
                {
                    "message": f"File uploaded successfully as {filename}",
                    "prediction": prediction,  # Include the predicted value
                }
            ),
            200,
        )
    except Exception as e:
        end_time = datetime.now()  # Record the end time in case of error
        print(f"[DEBUG] Request failed at: {end_time}")
        print(f"[DEBUG] Total processing time: {end_time - start_time}")
        print(f"[DEBUG] Error: {e}")

        return jsonify({"error": "Internal Server Error"}), 500


# Run the server
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000)
