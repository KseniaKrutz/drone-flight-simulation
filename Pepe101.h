#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Pepe101.generated.h"

UCLASS()
class TATNEFT_API APepe101 : public AActor
{
    GENERATED_BODY()

public:
    // Конструктор
    APepe101();

protected:
    // Метод, вызываемый при запуске игры
    virtual void BeginPlay() override;

public:
    // Метод, вызываемый каждый кадр
    virtual void Tick(float DeltaTime) override;

private:
    // Камера, прикрепленная к дрону
    UPROPERTY(VisibleAnywhere)
        UCameraComponent* CameraComponent;

    // Камера для игрока (Third Person)
    UPROPERTY(VisibleAnywhere)
        APlayerCameraManager* PlayerCameraManager;

    // Текущая позиция дрона
    FVector CurrentLocation;

    // Время с начала игры
    float TimeElapsed;

    // Скорость движения дрона
    float MovementSpeed;

    // Массив для хранения координат пути сканирования (шахматная доска)
    TArray<FVector> ScanPath;

    // Индекс текущей точки пути
    int32 CurrentWaypointIndex;

    // Флаг для завершения движения
    bool bShouldMove;

    // Функция для вычисления координат клеток шахматной доски с заданным размером сетки
    void CalculateGridCoordinates(int32 Rows, int32 Cols);

    // Функция для перемещения дрона по пути
    void MovePepe101(float DeltaTime);

    // Функция для захвата скриншота
    void TakeScreenshot(FString ScreenshotName);

    // Переключение на камеру дрона
    void SwitchToDroneCamera();

    // Переключение на камеру игрока
    void SwitchToPlayerCamera();

    // Обработка ввода
    void SetupInputComponent();
};
