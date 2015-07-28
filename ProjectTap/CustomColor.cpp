#include "ProjectTap.h"
#include "CustomColor.h"

FVector ColorHelpers::Tomato = FVector( 1.0f , 0.388f , 0.278f );
FVector ColorHelpers::LawnGreen = FVector( 0.486f , 0.988f , 0 );
FVector ColorHelpers::GetColorFromEnum( const CustomColor color )
{
	switch ( color )
	{
		case( CustomColor::LawnGreen ) :
			return LawnGreen;
		default:
			return Tomato;
	}
}