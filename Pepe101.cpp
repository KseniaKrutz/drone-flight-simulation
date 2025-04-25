#include "Pepe101.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Components/InputComponent.h"

// �����������
APepe101::APepe101()
{
    PrimaryActorTick.bCanEverTick = true;

    // ������� ��������� ������, ������� ����� ����������� � �����
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent); // ������ ����� �������� ��������� ��������� ����������

    // ��������� �������� � ������ ����������
    CurrentLocation = FVector(7250.f, 17410.f, 3500.f); // ��������� ������� ����� �� ������ 3000 �
    MovementSpeed = 2000.f; // �������� �����������
    TimeElapsed = 0.f;
    bShouldMove = true;
    CurrentWaypointIndex = 0;

    // ���������� ��������� ������ (�������� ������ �����)
    CalculateGridCoordinates(15, 15); // ����������� ����� �� 15x15 ������

    // ������������� ��������� �������
    SetActorLocation(CurrentLocation);
}

// ����� ���������� ��� ������� ����
void APepe101::BeginPlay()
{
    Super::BeginPlay();

    // ����� ������ ������ ��� ������������ ����� ��������
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerCameraManager = PlayerController->PlayerCameraManager;
    }

    // �������� ��������� ������ (�� ��������� ������)
    SwitchToPlayerCamera();

    // �������� ��������� �����
    SetupInputComponent();
}

// �����, ���������� ������ ����
void APepe101::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldMove)
    {
        MovePepe101(DeltaTime);
    }
}

// ������� ��� ���������� ��������� ������ ��������� �����
void APepe101::CalculateGridCoordinates(int32 Rows, int32 Cols)
{
    // �������� ����������, ������� ����� �������� ������� ������ �������
    FVector a1(7250.f, 17410.f, 3500.f); // a1 (�� ������ 3000 �)
    FVector a8(7250.f, 54640.f, 3500.f); // a8
    FVector h1(67220.f, 17410.f, 3500.f); // h1
    FVector h8(67220.f, 54640.f, 3500.f); // h8

    // ���������� ����� �������� �� ���� X � Y
    float XStep = (h1.X - a1.X) / (Cols - 1); // ���������� �� X
    float YStep = (a8.Y - a1.Y) / (Rows - 1); // ���������� �� Y

    // ��������� ������ ��������� ��� ������ ������ ��������� �����
    ScanPath.Empty(); // ������� ���������� ����������

    for (int32 Row = 0; Row < Rows; Row++)
    {
        for (int32 Col = 0; Col < Cols; Col++)
        {
            // ��� ������ ������ ���������� �� �������� ����������
            float X = a1.X + (Col * XStep);
            float Y = a1.Y + (Row * YStep);
            ScanPath.Add(FVector(X, Y, 3500.f)); // ��������� � ������ ��������� �� ������ 3000 �
        }
    }

    // ����������� ��������� ��� �������
    for (const FVector& Coord : ScanPath)
    {
        UE_LOG(LogTemp, Warning, TEXT("Grid Coord: %s"), *Coord.ToString());
    }
}

// ������� ��� ����������� ����� �� ����
void APepe101::MovePepe101(float DeltaTime)
{
    if (ScanPath.Num() == 0)
        return;

    FVector TargetLocation = ScanPath[CurrentWaypointIndex];
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

    // �������� ������� ��������
    UE_LOG(LogTemp, Warning, TEXT("Moving from %s to %s"), *CurrentLocation.ToString(), *TargetLocation.ToString());

    // ���������� ���� � ������� ������� �����
    CurrentLocation += Direction * MovementSpeed * DeltaTime;
    SetActorLocation(CurrentLocation);

    // ���������, �������� �� �� ������� �����
    if (FVector::Dist(CurrentLocation, TargetLocation) < 100.f) // �������� ����������� �� 100 ������
    {
        // ��������� ������� �������, �� ������ ���������
        int32 Row = FMath::FloorToInt((CurrentLocation.Y - 17410.f) / ((54640.f - 17410.f) / 14));  // ������� ������
        int32 Col = FMath::FloorToInt((CurrentLocation.X - 7250.f) / ((67220.f - 7250.f) / 14));  // ������� �������

        // ������� �������� � ��������� �� ����������� �������
        FString ScreenshotName = FString::Printf(TEXT("%d_%d.png"), Col + 1, Row + 1);
        TakeScreenshot(ScreenshotName);

        // ��������� � ��������� �����
        CurrentWaypointIndex++;

        // ���� ��� ����� ��������, ������������� ��������
        if (CurrentWaypointIndex >= ScanPath.Num())
        {
            bShouldMove = false;
            UE_LOG(LogTemp, Warning, TEXT("Movement completed."));
        }
    }
}

// ������� ��� ������� ���������
void APepe101::TakeScreenshot(FString ScreenshotName)
{
    // ���������, ���� �� ������ � �����
    if (CameraComponent)
    {
        // ��������, ��� �������� �������� � ������ �����
        FScreenshotRequest::RequestScreenshot(ScreenshotName, false, false);
    }

    // �������� ���������� � ��������� ������
    UE_LOG(LogTemp, Warning, TEXT("Taking screenshot: %s"), *ScreenshotName);
}

// ������������ �� ������ �����
void APepe101::SwitchToDroneCamera()
{
    if (PlayerCameraManager && CameraComponent)
    {
        // ����������� ������ �� ������ �����
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetViewTargetWithBlend(this, 0.5f);
        }
    }
}

// ������������ �� ������ ������
void APepe101::SwitchToPlayerCamera()
{
    if (PlayerCameraManager)
    {
        // ����������� ������ �� ������ ������
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetViewTargetWithBlend(PlayerController->GetPawn(), 0.5f);
        }
    }
}

// ����� ��� ��������� �����
void APepe101::SetupInputComponent()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        InputComponent = PlayerController->InputComponent;
        if (InputComponent)
        {
            // �������� �������� ��� ������������ ������ �� ������� "C"
            InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &APepe101::SwitchToDroneCamera);
        }
    }
}
