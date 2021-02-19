#pragma once

#include <stdint.h>
#include "math_def.h"

#define	MOTOR_PHASE_U_V_W		0x00
#define	MOTOR_PHASE_U_W_V		0x01
#define	MOTOR_PHASE_V_U_W		0x02
#define	MOTOR_PHASE_V_W_U		0x03
#define	MOTOR_PHASE_W_U_V		0x04
#define	MOTOR_PHASE_W_V_U		0x05

#define MOTOR_TYPE_DC			0x00
#define	MOTOR_TYPE_BLDC			0x01
#define	MOTOR_TYPE_SPMSM		0x02
#define	MOTOR_TYPE_IPMSM		0x03
#define	MOTOR_TYPE_STEP			0x04
#define	MOTOR_TYPE_REVOLUTE		0x00
#define	MOTOR_TYPE_LINEAR		0x10

class MotorProperty
{
public:
	MotorProperty(void);
	
	//	Motor Information
	uint8_t _motorType;
	
	float _R;						//	��				������ �Ǽ�(Armature Winding)��  ����
	float _Ld;						//	H				������ �Ǽ�(Armature Winding)�� d�� �δ��Ͻ�
	float _Lq;						//	H				������ �Ǽ�(Armature Winding)�� q�� �δ��Ͻ�
	float _Kt;						//	Nm/A			��ũ ���
	float _Ke;						//	V/(rad/s)		�������� ���
	float _Jm;						//	[kg.m^2]		Moment of Inertia
	float _Fv;						//	[Nm/(rad/s)]	Viscous Friction
	float _Fc;						//	[Nm]			Coulomb Friction
	uint8_t _motorPhase;			//	

	float _maximumCurrent;			//	[A]				�ִ�����
	float _ratedCurrent;			//	[A]				��������
	float _ratedTorque;				//	[Nm]			������ũ
	float _maximumSpeed;			//	[rad/sec]		���ݼӵ�(�ִ�ӵ�)
	float _maximumAcceleration;		//	[rad/sec^2]		�ִ밡�ӵ�
	float _maximumDeceleration;		//	[rad/sec^2]		�ִ밨�ӵ�
	
	uint8_t _polePair;
	int32_t _encoderResolution;
	
	float _pulse2deg;
	float _deg2pulse;
	float _pulse2rad;
	float _rad2pulse;
	
	int8_t SetRatedCurrent(float ratedCurrent);
	int8_t SetRatedTorque(float ratedTorque);
	int8_t SetMaximumSpeed(float maximumSpeed);
	int8_t SetPolePair(uint8_t polePair);
	int8_t SetEncoderResolution(int32_t encoderResolution);
	int8_t SetMotorPhase(uint8_t motorPhase);
	
	//	Voltage Control
	float _actualElecTheta;
	float _prevActualElecTheta;
	float _targetElecTheta;
	uint8_t _hallSensorPatternBuf[12][6];
	uint8_t _hallSensorPattern[6];
	void ResetHallSensorPattern(void);
	int8_t SearchHallSensorPattern(uint8_t hallSensorPattern, float angle);
	
	
};
