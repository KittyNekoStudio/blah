package com.blah;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javafx.scene.Scene;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.control.TextArea;
import javafx.scene.layout.BorderPane;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

public class Editor {

	private TextArea textArea;
	private Stage stage;
	private String currentFileName = null;

	public Editor(Stage stage) {
		this.stage = stage;

		var textArea = new TextArea();
		this.textArea = textArea;
	}

	public void init() {
		textArea.setWrapText(true);

		var menuBar = createMenuBar();

		var root = new BorderPane();
		root.setTop(menuBar);
		root.setCenter(textArea);

		var scene = new Scene(root, 800, 800);

		this.stage.setScene(scene);
	}

	public MenuBar createMenuBar() {
		var menuBar = new MenuBar();

		var fileMenu = new Menu("File");
		var openItem = new MenuItem("Open");
		var saveItem = new MenuItem("Save");
		var exitItem = new MenuItem("Exit");

		fileMenu.getItems().addAll(openItem, 
			saveItem, 
			new SeparatorMenuItem(), 
			exitItem);

		openItem.setOnAction(e -> openFile());
		saveItem.setOnAction(e -> saveFile());
		exitItem.setOnAction(e -> stage.close());

		menuBar.getMenus().add(fileMenu);

		return menuBar;
	}

	public void openFile() {
		var fileChooser = new FileChooser();
		fileChooser.setTitle("Open file");

		var selectedFile = fileChooser.showOpenDialog(stage);
		this.currentFileName = selectedFile.getName();

		if (selectedFile != null) {
			try (var reader = new BufferedReader(new FileReader(selectedFile))) {
				var buffer = new StringBuilder();
				String line;

				while ((line = reader.readLine()) != null) {
					buffer.append(line).append("\n");
				}

				textArea.setText(buffer.toString());
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void saveFile() {
		var fileChooser = new FileChooser();
		fileChooser.setTitle("Save file");

		fileChooser.setInitialFileName(this.currentFileName);
		var selectedFile = fileChooser.showSaveDialog(stage);

		if (selectedFile != null) {
			try (var writer = new FileWriter(selectedFile)) {
				writer.write(textArea.getText());
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

}
