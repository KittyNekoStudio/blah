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
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.robot.Robot;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

public class Editor {

	private TextArea textArea;
	private Stage stage;
	private String currentFileName = null;
	private Modes mode;
	private boolean checkedMode = false;
	public enum Modes {
		INSERT,
		NORMAL
	}

	public Editor(Stage stage) {
		this.stage = stage;
		this.mode = Modes.NORMAL;

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

		textArea.addEventHandler(KeyEvent.KEY_PRESSED, (key) -> {
			var keyCode = key.getCode();
			if (keyCode == KeyCode.I && this.mode == Modes.NORMAL) {
				this.mode = Modes.INSERT;
				checkedMode = false;
				// TODO! change this to just escape
			} else if (keyCode == KeyCode.CAPS | keyCode == KeyCode.ESCAPE && this.mode == Modes.INSERT) { 
				this.mode = Modes.NORMAL;
				checkedMode = false;
			}
		});

		textArea.addEventFilter(KeyEvent.ANY, (e) -> {
			if (checkedMode == false && isNormal()) {
				textArea.setEditable(false);
				checkedMode = true;
			}

			if (checkedMode == false && isInsert()) {
				textArea.setEditable(true);
				checkedMode = true;
				// We consume otherwise it would print the character 'i' to the textArea
				e.consume();
			}
		});

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

	public Modes getMode() {
		return this.mode;
	}

	public boolean isNormal() {
		return getMode() == Modes.NORMAL;
	}

	public boolean isInsert() {
		return getMode() == Modes.INSERT;
	}
}
