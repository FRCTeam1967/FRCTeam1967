#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "LightsKey.h"

using namespace frc;

class LightsController
{
	public:
		LightsController(int pwmChannel);
		virtual ~LightsController();

		void SetColor(int color, int mode);
	
	private:
		PWM*lightController;
		int color;
		int mode;
};