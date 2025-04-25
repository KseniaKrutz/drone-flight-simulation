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
    // �����������
    APepe101();

protected:
    // �����, ���������� ��� ������� ����
    virtual void BeginPlay() override;

public:
    // �����, ���������� ������ ����
    virtual void Tick(float DeltaTime) override;

private:
    // ������, ������������� � �����
    UPROPERTY(VisibleAnywhere)
        UCameraComponent* CameraComponent;

    // ������ ��� ������ (Third Person)
    UPROPERTY(VisibleAnywhere)
        APlayerCameraManager* PlayerCameraManager;

    // ������� ������� �����
    FVector CurrentLocation;

    // ����� � ������ ����
    float TimeElapsed;

    // �������� �������� �����
    float MovementSpeed;

    // ������ ��� �������� ��������� ���� ������������ (��������� �����)
    TArray<FVector> ScanPath;

    // ������ ������� ����� ����
    int32 CurrentWaypointIndex;

    // ���� ��� ���������� ��������
    bool bShouldMove;

    // ������� ��� ���������� ��������� ������ ��������� ����� � �������� �������� �����
    void CalculateGridCoordinates(int32 Rows, int32 Cols);

    // ������� ��� ����������� ����� �� ����
    void MovePepe101(float DeltaTime);

    // ������� ��� ������� ���������
    void TakeScreenshot(FString ScreenshotName);

    // ������������ �� ������ �����
    void SwitchToDroneCamera();

    // ������������ �� ������ ������
    void SwitchToPlayerCamera();

    // ��������� �����
    void SetupInputComponent();
};
