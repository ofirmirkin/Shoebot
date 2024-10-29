from PIL import Image
import torch
from torch import nn
from torchvision import transforms, models
import matplotlib.pyplot as plt
import numpy as np


image_x, image_y = 240, 320
classes = {0: "Shoes", 1: "Slippers", 2: "None"}


class CustomResNet(nn.Module):
    def __init__(self, num_classes=3):
        super(CustomResNet, self).__init__()
        # Load a pretrained ResNet18 model
        self.resnet = models.resnet18(weights=models.ResNet18_Weights.DEFAULT)

        # Replace the final fully connected layer
        in_features = self.resnet.fc.in_features
        self.resnet.fc = nn.Linear(in_features, num_classes)

        # Add softmax layer
        self.softmax = nn.Softmax(dim=1)

    def forward(self, x):
        x = self.resnet(x)
        return self.softmax(x)


class Predictor:
    def __init__(self, model_path):
        self.model = CustomResNet(num_classes=3)
        self.model.load_state_dict(torch.load(model_path, weights_only=True))
        self.model.eval()

        self.preprocess = transforms.Compose(
            [
                transforms.Resize(
                    (image_x, image_y)
                ),  # Resize image to the required size
                transforms.ToTensor(),  # Convert image to tensor
            ]
        )

    def predict_image(self, image_path):
        # Load and preprocess the image
        image = Image.open(image_path)
        image = self.preprocess(image)
        image = image.unsqueeze(0)  # Add batch dimension

        # Perform inference
        with torch.no_grad():
            output = self.model(image)
            print(f"{output[0, :2]=}")

        # Get the predicted class
        predicted = torch.argmax(output)
        print(f"Predicted: {classes[predicted.item()]}")
        print(f"Predicted: {predicted.item()}")

        return predicted.item()
