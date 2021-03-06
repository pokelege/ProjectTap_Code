// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "PortalTile.h"
#include "OffensiveTiles/Laser.h"
#include "Pawns/BallPawn.h"
#include "Pawns/PawnCastingTrigger.h"
#include "MagnetTile.h"
const GroundableInfo APortalTile::groundableInfo = GroundableInfo( FVector( 0 , 0 , 40 ) , true );
APortalTile::APortalTile()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh( TEXT( "/Game/Models/Portal" ) );
	TileMesh->SetStaticMesh( mesh.Object );
	TileMesh->SetMobility( EComponentMobility::Static );
	BoxCollision->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );
	BoxCollision->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	BoxCollision->SetWorldScale3D(FVector(1.0f));
	BoxCollision->SetBoxExtent(FVector(1.0f));

	bluePortalTrigger = CreateDefaultSubobject<UBoxComponent>( TEXT( "BlueTrigger" ) );
	orangePortalTrigger = CreateDefaultSubobject<UBoxComponent>( TEXT( "OrangeTrigger" ) );

	bluePortalTrigger->AttachTo( RootComponent );
	bluePortalTrigger->bGenerateOverlapEvents = true;

	orangePortalTrigger->AttachTo( RootComponent );
	orangePortalTrigger->bGenerateOverlapEvents = true;

	bluePortalTrigger->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECollisionResponse::ECR_Block );
	orangePortalTrigger->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECollisionResponse::ECR_Block );

	beginBlueOverlap.BindUFunction( this , "OnBlueBeginTriggerOverlap" );
	bluePortalTrigger->OnComponentBeginOverlap.Add( beginBlueOverlap );


	endBlueOverlap.BindUFunction( this , "OnBlueEndTriggerOverlap" );
	bluePortalTrigger->OnComponentEndOverlap.Add( endBlueOverlap );

	beginOrangeOverlap.BindUFunction( this , "OnOrangeBeginTriggerOverlap" );
	orangePortalTrigger->OnComponentBeginOverlap.Add( beginOrangeOverlap );

	endOrangeOverlap.BindUFunction( this , "OnOrangeEndTriggerOverlap" );
	orangePortalTrigger->OnComponentEndOverlap.Add( endOrangeOverlap );

	GeneratePortalCollision();
	AdjustOrientationAndTriggerBoxes();

	ConstructorHelpers::FObjectFinder<USoundWave> teleportSoundFile( TEXT( "/Game/Sound/S_Teleport" ) );
	teleportSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Activate Sound" ) );
	teleportSound->SetSound( teleportSoundFile.Object );
	teleportSound->bAutoActivate = false;
	teleportSound->AttachTo( BoxCollision );
}

const GroundableInfo* APortalTile::GetGroundableInfo() const
{
	return &APortalTile::groundableInfo;
}

void APortalTile::AdjustOrientationAndTriggerBoxes()
{
	bluePortalTrigger->SetRelativeLocation( FVector( -5.0f , 0.f , 0.f ) );
	orangePortalTrigger->SetRelativeLocation( FVector( 5.f , 0.f , 0.f ) );

	bluePortalTrigger->SetBoxExtent( FVector( 3.f , 25.f , 30.f ) );

	orangePortalTrigger->SetBoxExtent( FVector( 3.f , 25.f , 30.f ) );

	orangePortalTrigger->SetCollisionResponseToAllChannels( ECR_Ignore );
	orangePortalTrigger->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	orangePortalTrigger->bGenerateOverlapEvents = true;
	orangePortalTrigger->SetNotifyRigidBodyCollision( false );
	orangePortalTrigger->SetCollisionObjectType( ECC_Portal );
	orangePortalTrigger->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECR_Overlap );
	orangePortalTrigger->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECR_Block );

	bluePortalTrigger->SetCollisionResponseToAllChannels( ECR_Ignore );
	bluePortalTrigger->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	bluePortalTrigger->bGenerateOverlapEvents = true;
	bluePortalTrigger->SetNotifyRigidBodyCollision( false );
	bluePortalTrigger->SetCollisionObjectType( ECC_Portal );
	bluePortalTrigger->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECR_Overlap );
	bluePortalTrigger->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECR_Block );
}

void APortalTile::GeneratePortalCollision()
{
	auto left = CreateDefaultSubobject<UBoxComponent>( TEXT( "left" ) );


	auto right = CreateDefaultSubobject<UBoxComponent>( TEXT( "right" ) );


	auto top = CreateDefaultSubobject<UBoxComponent>( TEXT( "top" ) );


	auto bottom = CreateDefaultSubobject<UBoxComponent>( TEXT( "bottom" ) );

	left->AttachTo( RootComponent );
	right->AttachTo( RootComponent );
	top->AttachTo( RootComponent );
	bottom->AttachTo( RootComponent );

	SetMeshCollisionProperty( left );
	SetMeshCollisionProperty( right );
	SetMeshCollisionProperty( top );
	SetMeshCollisionProperty( bottom );

	left->SetRelativeLocation( FVector( 0.000000 , -38.000000 , 0.000000 ) );
	left->SetBoxExtent( FVector( 40.000000 , 2.000000 , 40.000000 ) );
	right->SetRelativeLocation( FVector( 0.000000 , 38.000000 , 0.000000 ) );
	right->SetBoxExtent( FVector( 40.000000 , 2.000000 , 40.000000 ) );
	top->SetRelativeLocation( FVector( 0.000000 , 0.000000 , 38.000000 ) );
	top->SetBoxExtent( FVector( 40.000000 , 40.000000 , 2.000000 ) );
	bottom->SetRelativeLocation( FVector( 0.000000 , 0.000000 , -42.000000 ) );
	bottom->SetBoxExtent( FVector( 40.000000 , 40.000000 , 2.000000 ) );
}

void APortalTile::SetMeshCollisionProperty( UBoxComponent* box )
{
	box->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	box->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );
	box->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECollisionResponse::ECR_Block );
	box->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECollisionResponse::ECR_Block );
	box->SetCollisionResponseToChannel( ECollisionChannel::ECC_Visibility , ECollisionResponse::ECR_Block );
	box->SetCollisionObjectType( ECC_Portal );

}

void APortalTile::BeginPlay()
{
	Super::BeginPlay();
	if ( material != nullptr )
	{
		material->SetVectorParameterValue( TEXT( "BaseColor" ) , ColorHelpers::GetColorFromEnum( color ) );
		material->SetVectorParameterValue( TEXT( "BaseColorHighlighted" ) , ColorHelpers::GetColorFromEnum( color ) );
	}
}

void APortalTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

OffsetInfo APortalTile::getOffsetInfo()
{
	OffsetInfo data;
	data.offsetForCollision = FVector( 0.0f , 0.0f , 50.0f );
	data.scaleForCollision = FVector( 1.2f , 1.2f , 6.0f );
	data.offsetForCarryOn = FVector( 0.0f , 0.0f , 50.0f );
	return data;
}

void APortalTile::OnBlueBeginTriggerOverlap( AActor* OtherActor ,
											 UPrimitiveComponent* OtherComp ,
											 int32 OtherBodyIndex ,
											 bool bFromSweep ,
											 const FHitResult & SweepResult )
{
	if ( enabled )
	{
		if ( enteredPortal )
		{
			if ( auto a = Cast<ABallPawn>( OtherActor ) )
			{
				a->setInvincibility( true );
				TransportBallToOrange( a );
			}
		}
		else
		{
			enteredPortal = true;
		}
	}
}

void APortalTile::OnOrangeBeginTriggerOverlap( AActor* OtherActor ,
											   UPrimitiveComponent* OtherComp ,
											   int32 OtherBodyIndex ,
											   bool bFromSweep ,
											   const FHitResult & SweepResult )
{
	if ( enabled )
	{
		if ( enteredPortal )
		{
			if ( auto a = Cast<ABallPawn>( OtherActor ) )
			{
				a->setInvincibility( true );
				TransportBallToBlue( a );
			}
		}
		else
		{
			enteredPortal = true;
		}
	}
}

void APortalTile::OnBlueEndTriggerOverlap( AActor* OtherActor ,
										   UPrimitiveComponent* OtherComp ,
										   int32 OtherBodyIndex ,
										   bool bFromSweep ,
										   const FHitResult & SweepResult )
{
	enteredPortal = false;
	leftBluePortal = true;
	ProcessBallEndfOverlap( OtherActor );
	Enable();
}


void APortalTile::OnOrangeEndTriggerOverlap( AActor* OtherActor ,
											 UPrimitiveComponent* OtherComp ,
											 int32 OtherBodyIndex ,
											 bool bFromSweep ,
											 const FHitResult & SweepResult )
{
	enteredPortal = false;
	leftOrangePortal = true;

	ProcessBallEndfOverlap( OtherActor );

	Enable();
}

void APortalTile::Enable()
{
	if ( leftOrangePortal && leftBluePortal )
	{
		enabled = true;
		leftOrangePortal = false;
		leftBluePortal = false;
	}
}

void APortalTile::ProcessBallEndfOverlap( AActor* actor )
{
	if ( ( leftOrangePortal || leftBluePortal ) && !enabled )
	{
		auto ball = Cast<ABallPawn>( actor );
		auto trigger = Cast<APawnCastingTrigger>( actor );
		if ( ball != nullptr )
		{
			ball->setInvincibility( false );
		}
		else if ( trigger != nullptr )
		{
			trigger->currentPawn->setInvincibility( false );
		}

	}
}


void APortalTile::TransportBallToOrange( ABallPawn* pawn )
{
	if ( otherPortal != nullptr )
	{
		otherPortal->enabled = false;
		auto transportLocation = otherPortal->GetActorLocation();
		transportLocation.Z += 3.0f;
		pawn->SetActorLocation( transportLocation );
		auto newVelMag = pawn->ballCollision->GetPhysicsLinearVelocity().Size();
		auto newVel = newVelMag * velocityMultiplier * otherPortal->GetActorForwardVector();
		pawn->ballCollision->SetPhysicsLinearVelocity( newVel );
		pawn->ballCollision->SetPhysicsAngularVelocity( FVector( 0.0f ) );
		teleportSound->Play();
	}
}

void APortalTile::TransportBallToBlue( ABallPawn* pawn )
{
	if ( otherPortal != nullptr )
	{
		otherPortal->enabled = false;
		auto transportLocation = otherPortal->GetActorLocation();
		transportLocation.Z += 3.0f;
		pawn->SetActorLocation( transportLocation );
		auto newVelMag = pawn->ballCollision->GetPhysicsLinearVelocity().Size();
		auto newVel = newVelMag * velocityMultiplier * -otherPortal->GetActorForwardVector();
		pawn->ballCollision->SetPhysicsLinearVelocity( newVel );
		pawn->ballCollision->SetPhysicsAngularVelocity( FVector( 0.0f ) );
		teleportSound->Play();
	}
}

void APortalTile::GetLaserPortalTransportedLocation( UPrimitiveComponent* hit4PportalTrigger , FVector& newDir , FVector& newPos )
{
	if ( otherPortal != nullptr && hit4PportalTrigger != nullptr )
	{
		bool isPortalTrigger = hit4PportalTrigger == orangePortalTrigger || hit4PportalTrigger == bluePortalTrigger;

		if ( isPortalTrigger )
		{
			bool isOrangePortal = hit4PportalTrigger == orangePortalTrigger;
			auto newLaserPos = isOrangePortal ?
				otherPortal->orangePortalTrigger->GetComponentLocation() :
				otherPortal->bluePortalTrigger->GetComponentLocation();

			newDir = isOrangePortal ? otherPortal->GetActorRotation().Vector() : -otherPortal->GetActorRotation().Vector();
			newPos = newLaserPos + newDir * 10.0f;
		}
	}
}

void APortalTile::GetMagnetPortalTransportedLocation( UPrimitiveComponent* hitPportalTrigger , FVector& newDir , FVector& newPos )
{
	GetLaserPortalTransportedLocation( hitPportalTrigger , newDir , newPos );
}

#if WITH_EDITOR
void APortalTile::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	Super::PostEditChangeProperty( PropertyChangedEvent );

	if ( PropertyChangedEvent.Property != nullptr )
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetNameCPP();

		//when currentEditorPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if ( pName.Equals( "otherPortal" ) )
		{
			if ( otherPortal != nullptr )
			{
				otherPortal->otherPortal = this;
				otherPortal->color = color;
			}
			
		}
		else if ( pName.Equals( "color" ) )
		{
			if ( otherPortal != nullptr )
			{
				otherPortal->color = color;
			}
		}
	}

}
#endif
