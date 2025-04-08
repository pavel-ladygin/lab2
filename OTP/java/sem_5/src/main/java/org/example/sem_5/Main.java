package org.example.demo3;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

public class Main extends Application {
    @Override
    public void start(Stage stage) {
        Pane pane = new Pane();
        Scene scene = new Scene(pane, 1980, 1040);
        stage.setTitle("Hello");
        stage.setScene(scene);
        stage.show();
        Button button = new Button("Кнопка1");
        Button button1 = new Button("Кнопка2");
        Button button2 = new Button("Кнопка3");
        Button button3 = new Button("Площадь пересечения");

        button.setMinSize(120,60);
        button1.setMinSize(480,240);
        button2.setMinSize(120,240);

        button.setLayoutX(100); button.setLayoutY(350);
        button1.setLayoutX(500); button1.setLayoutY(350);
        button2.setLayoutX(1200); button2.setLayoutY(350);
        MovingWhenConfronting movingWhenConfronting = new MovingWhenConfronting();


        MousePressedCoords mousePressedCoords = new MousePressedCoords();
        button.setOnMouseClicked(mouseEvent -> {
            mousePressedCoords.setX(mouseEvent.getSceneX() - button.getLayoutX());
            mousePressedCoords.setY(mouseEvent.getSceneY() - button.getLayoutY());
        });
        button.setOnMouseDragged(mouseEvent -> {

            button.setLayoutX(mouseEvent.getSceneX() - mousePressedCoords.getX());
            button.setLayoutY(mouseEvent.getSceneY() - mousePressedCoords.getY());

            double y = movingWhenConfronting.intersection(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button.getLayoutX(),button.getLayoutY(),120,60);

            button3.setText("" +(int)y);

        });

        MousePressedCoords mousePressedCoords1 = new MousePressedCoords();
        button1.setOnMouseClicked(mouseEvent -> {
            mousePressedCoords1.setX(mouseEvent.getSceneX() - button1.getLayoutX());
            mousePressedCoords1.setY(mouseEvent.getSceneY() - button1.getLayoutY());
        });
        button1.setOnMouseDragged(mouseEvent -> {
            button1.setLayoutX(mouseEvent.getSceneX() - mousePressedCoords1.getX());
            button1.setLayoutY(mouseEvent.getSceneY() - mousePressedCoords1.getY());
            button.setLayoutX(movingWhenConfronting.movingCoordsX(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button.getLayoutX(),button.getLayoutY(),120,60));
            button.setLayoutY(movingWhenConfronting.movingCoordsY(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button.getLayoutX(),button.getLayoutY(),120,60));
            button2.setLayoutX(movingWhenConfronting.movingCoordsX(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button2.getLayoutX(),button2.getLayoutY(),120,240));
            button2.setLayoutY(movingWhenConfronting.movingCoordsY(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button2.getLayoutX(),button2.getLayoutY(),120,240));
            button2.setLayoutX(movingWhenConfronting.movingCoordsX(
                    button.getLayoutX(),button.getLayoutY(),120,60,
                    button2.getLayoutX(),button2.getLayoutY(),120,240));
            button2.setLayoutY(movingWhenConfronting.movingCoordsY(
                    button.getLayoutX(),button.getLayoutY(),120,60,
                    button2.getLayoutX(),button2.getLayoutY(),120,240));
        });

        MousePressedCoords mousePressedCoords2 = new MousePressedCoords();
        button2.setOnMouseClicked(mouseEvent -> {
            mousePressedCoords2.setX(mouseEvent.getSceneX() - button2.getLayoutX());
            mousePressedCoords2.setY(mouseEvent.getSceneY() - button2.getLayoutY());
        });
        button2.setOnMouseDragged(mouseEvent -> {
            button2.setLayoutX(mouseEvent.getSceneX() - mousePressedCoords2.getX());
            button2.setLayoutY(mouseEvent.getSceneY() - mousePressedCoords2.getY());

            button.setLayoutX(movingWhenConfronting.movingCoordsX(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button.getLayoutX(),button.getLayoutY(),120,60));
            button.setLayoutY(movingWhenConfronting.movingCoordsY(
                    button1.getLayoutX(),button1.getLayoutY(),480,240,
                    button.getLayoutX(),button.getLayoutY(),120,60));
            button1.setLayoutX(movingWhenConfronting.movingCoordsX(
                    button2.getLayoutX(),button2.getLayoutY(),120,240,
                    button1.getLayoutX(),button1.getLayoutY(),480,240));
            button1.setLayoutY(movingWhenConfronting.movingCoordsY(
                    button2.getLayoutX(),button2.getLayoutY(),120,240,
                    button1.getLayoutX(),button1.getLayoutY(),480,240));
            button.setLayoutX(movingWhenConfronting.movingCoordsX(
                    button2.getLayoutX(),button2.getLayoutY(),120,240,
                    button.getLayoutX(),button.getLayoutY(),120,60));
            button.setLayoutY(movingWhenConfronting.movingCoordsY(
                    button2.getLayoutX(),button2.getLayoutY(),120,240,
                    button.getLayoutX(),button.getLayoutY(),120,60));
        });

        pane.getChildren().add(button);
        pane.getChildren().add(button1);
        pane.getChildren().add(button2);
        pane.getChildren().add(button3);
    }
    public static void main(String[] args) {

        launch();
    }
}