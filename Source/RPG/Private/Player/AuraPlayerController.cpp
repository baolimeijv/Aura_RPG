#include "Player/AuraPlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include"Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//可复制的实体，用于服务器响应数据更新，并将这些更新复制给客户端
	bReplicates = true;

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	//A:无效点击                 LastActor==nullptr&&ThisActor==nullptr     no procession
	//B:第一次点中敌人           LastActor ==nullptr&&ThisActor is valid    ThisActor HighlightActor
	//C:悬停敌人                 LastActor is valid && ThisActor is valid   no procession
	//D:离开敌人迅速到另一个敌人 LastActor is vaild && ThisActor is valid   LastActor UnHighlightActor ThisActor HighlightActor
	//E:离开敌人到不是敌人       LastActor is valid && ThisActor ==nullptr  ThisActor UnHighlightActor

	if (!ThisActor)
	{	//E
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
	}
	else
	{
		if (!LastActor)//B
		{
			ThisActor->HighlightActor();
		}
		else
		{
			if (ThisActor != LastActor)//D
			{
				ThisActor->HighlightActor();
				LastActor->UnHighlightActor();
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);


}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRoation(0.f, Rotation.Yaw, 0.f);

	const FVector Forwarod = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRoation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		ControllerPawn->AddMovementInput(Forwarod, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(Right, InputAxisVector.X);
	}
}


