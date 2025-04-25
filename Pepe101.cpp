#include "Pepe101.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Components/InputComponent.h"

// Конструктор
APepe101::APepe101()
{
    PrimaryActorTick.bCanEverTick = true;

    // Создаем компонент камеры, которая будет прикреплена к дрону
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent); // Камера будет дочерним элементом корневого компонента

    // Настройки скорости и других переменных
    CurrentLocation = FVector(7250.f, 17410.f, 3500.f); // Начальная позиция дрона на высоте 3000 м
    MovementSpeed = 2000.f; // Скорость перемещения
    TimeElapsed = 0.f;
    bShouldMove = true;
    CurrentWaypointIndex = 0;

    // Вычисление координат клеток (увеличим размер сетки)
    CalculateGridCoordinates(15, 15); // Увеличиваем сетку до 15x15 клеток

    // Устанавливаем начальную позицию
    SetActorLocation(CurrentLocation);
}

// Метод вызываемый при запуске игры
void APepe101::BeginPlay()
{
    Super::BeginPlay();

    // Найти камеру игрока для переключения между камерами
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerCameraManager = PlayerController->PlayerCameraManager;
    }

    // Включаем начальную камеру (по умолчанию игрока)
    SwitchToPlayerCamera();

    // Настроим обработку ввода
    SetupInputComponent();
}

// Метод, вызываемый каждый кадр
void APepe101::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldMove)
    {
        MovePepe101(DeltaTime);
    }
}

// Функция для вычисления координат клеток шахматной доски
void APepe101::CalculateGridCoordinates(int32 Rows, int32 Cols)
{
    // Конечные координаты, которые будут задавать размеры вашего региона
    FVector a1(7250.f, 17410.f, 3500.f); // a1 (на высоте 3000 м)
    FVector a8(7250.f, 54640.f, 3500.f); // a8
    FVector h1(67220.f, 17410.f, 3500.f); // h1
    FVector h8(67220.f, 54640.f, 3500.f); // h8

    // Расстояние между клетками по осям X и Y
    float XStep = (h1.X - a1.X) / (Cols - 1); // Расстояние по X
    float YStep = (a8.Y - a1.Y) / (Rows - 1); // Расстояние по Y

    // Заполняем массив координат для каждой клетки шахматной доски
    ScanPath.Empty(); // Очистим предыдущие координаты

    for (int32 Row = 0; Row < Rows; Row++)
    {
        for (int32 Col = 0; Col < Cols; Col++)
        {
            // Для каждой клетки определяем ее реальные координаты
            float X = a1.X + (Col * XStep);
            float Y = a1.Y + (Row * YStep);
            ScanPath.Add(FVector(X, Y, 3500.f)); // Добавляем в массив координат на высоте 3000 м
        }
    }

    // Логирование координат для отладки
    for (const FVector& Coord : ScanPath)
    {
        UE_LOG(LogTemp, Warning, TEXT("Grid Coord: %s"), *Coord.ToString());
    }
}

// Функция для перемещения дрона по пути
void APepe101::MovePepe101(float DeltaTime)
{
    if (ScanPath.Num() == 0)
        return;

    FVector TargetLocation = ScanPath[CurrentWaypointIndex];
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

    // Логируем текущее движение
    UE_LOG(LogTemp, Warning, TEXT("Moving from %s to %s"), *CurrentLocation.ToString(), *TargetLocation.ToString());

    // Перемещаем дрон в сторону целевой точки
    CurrentLocation += Direction * MovementSpeed * DeltaTime;
    SetActorLocation(CurrentLocation);

    // Проверяем, достигли ли мы целевой точки
    if (FVector::Dist(CurrentLocation, TargetLocation) < 100.f) // Увеличим погрешность до 100 единиц
    {
        // Вычисляем индексы сектора, на основе координат
        int32 Row = FMath::FloorToInt((CurrentLocation.Y - 17410.f) / ((54640.f - 17410.f) / 14));  // Находим строку
        int32 Col = FMath::FloorToInt((CurrentLocation.X - 7250.f) / ((67220.f - 7250.f) / 14));  // Находим колонку

        // Снимаем скриншот с названием по координатам сектора
        FString ScreenshotName = FString::Printf(TEXT("%d_%d.png"), Col + 1, Row + 1);
        TakeScreenshot(ScreenshotName);

        // Переходим к следующей точке
        CurrentWaypointIndex++;

        // Если все точки пройдены, останавливаем движение
        if (CurrentWaypointIndex >= ScanPath.Num())
        {
            bShouldMove = false;
            UE_LOG(LogTemp, Warning, TEXT("Movement completed."));
        }
    }
}

// Функция для захвата скриншота
void APepe101::TakeScreenshot(FString ScreenshotName)
{
    // Проверяем, есть ли камера у дрона
    if (CameraComponent)
    {
        // Убедимся, что скриншот делается с камеры дрона
        FScreenshotRequest::RequestScreenshot(ScreenshotName, false, false);
    }

    // Логируем информацию о сделанном снимке
    UE_LOG(LogTemp, Warning, TEXT("Taking screenshot: %s"), *ScreenshotName);
}

// Переключение на камеру дрона
void APepe101::SwitchToDroneCamera()
{
    if (PlayerCameraManager && CameraComponent)
    {
        // Переключаем камеру на камеру дрона
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetViewTargetWithBlend(this, 0.5f);
        }
    }
}

// Переключение на камеру игрока
void APepe101::SwitchToPlayerCamera()
{
    if (PlayerCameraManager)
    {
        // Переключаем камеру на камеру игрока
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetViewTargetWithBlend(PlayerController->GetPawn(), 0.5f);
        }
    }
}

// Метод для настройки ввода
void APepe101::SetupInputComponent()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        InputComponent = PlayerController->InputComponent;
        if (InputComponent)
        {
            // Привязка действия для переключения камеры на клавишу "C"
            InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &APepe101::SwitchToDroneCamera);
        }
    }
}
