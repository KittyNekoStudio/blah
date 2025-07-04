package com.blah;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;


public class Blah extends Application {

	public Blah() {

	}

	@Override
	public void start(Stage stage) {
		var color = new Color(0.0, 0.0, 1.0, 1.0);
		var circle = new Circle(40, 40, 30, color);
		var circle2 = new Circle(500, 300, 30, color);
		var root = new Group(circle, circle2);
		var scene = new Scene(root, 800, 800);

		stage.setScene(scene);
		stage.show();
	}

	public static void main(String[] args) {
		launch();
	}
}
