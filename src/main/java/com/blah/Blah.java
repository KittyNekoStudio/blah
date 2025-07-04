package com.blah;

import javafx.application.Application;
import javafx.stage.Stage;


public class Blah extends Application {

	private Editor editor;

	public Blah() {}

	public Blah(Stage stage) {
		stage.setTitle("blah");
		this.editor = new Editor(stage);
	}

	@Override
	public void start(Stage stage) {
		var blah = new Blah(stage);
		blah.editor.init();
		stage.show();
	}

	public static void main(String[] args) {
		launch();
	}
}
