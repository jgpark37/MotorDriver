#pragma once

#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "math_def.h"
#include "arm_math.h"

#define sin		arm_sin_f32
#define cos		arm_cos_f32
/*
class SinusoidalProfile {
private:
	float dt;
	float pulse2rad;
	float rad2pulse;

	int32_t  p_0;	// ���� ��ġ
	int32_t  p_e;		// �� ��ġ
	float v_0;		// �ʱ� �ӵ�
	float v_x;		// �ְ� �ӵ�(��ġ ��������), ���� �ӵ�(�ӵ� ��������)

	float S, S1a, S1b, S1, S2;
	float T1, T2, T3;

	int32_t   mode;		// ���������� 3�ܰ� ���(0-����, 1-����, 2-����, -1-�������� ����)
	float t;		// ���������� ����� �ð�
	float m_pi;

public:
	SinusoidalProfile (int32_t f_) : dt(1.f/f_), mode(-1), t(0) { }

	void Reset ()
	{
		mode = -1;
	}

	void SetParam (float pulse2rad_, float rad2pulse_, float m_pi_)
	{
		pulse2rad = pulse2rad_;
		rad2pulse = rad2pulse_;
		m_pi = m_pi_;
	}

	void InitVelocityProfile (float v_tgt, float v_ini, float v_max, float a, float d, float J)
	{
		assert (J     > EPS);
		assert (a     > EPS);
		assert (d     > EPS);
		assert (v_max > EPS);

		v_0 = v_ini;
		v_x = Limit_f (v_tgt, -v_max, v_max);

		float a0 = (fabs(v_x) > fabs(v_0)) ? a : d;
		float a1 = a0;

		for (int i=0; i<3; i++) {
			float dv = v_x - v_0;
			T1  = max(dt, (m_pi/2.0f)*fabs(dv/a1));
			S1a = 0.5f*dv*T1;
			S1b = v_0*T1;

			S1  = S1a + S1b;
			T2  = T3 = 0;

			bool  repeat = false;
			float j1 = (T1 > EPS) ? fabs(m_pi*m_pi*S1a/(T1*T1*T1)) : J;

			if      (j1 > J*2          ) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }
			else if (j1 < J/2 && a1 < a0) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }

			if (!repeat) break;
		}

		mode = 0;
		t = 0;
	}

	void InitPositionProfile (long p_del, long p_ini, float v_ini, float v_max, float a, float d, float J)
	{
		assert (J     > EPS);
		assert (a     > EPS);
		assert (d     > EPS);
		assert (v_max > EPS);

		v_0 = v_ini;
		p_0	= p_ini;
		p_e = p_ini + p_del;
		S	= p_del*pulse2rad;

		float a1 = a;
		float a2 = d;

		// ���� �ӵ� v_0���� ���ӵ� a_max�� �����Ͽ� ������ ���, �̵��Ÿ� S�� ������� üũ�Ѵ�.
		// ���� �ְ�ӵ��� ����ԵǸ�, v_x ���� �ְ� �ӵ��� �ΰ� �ٸ� �Ķ���͸� �����ؾ��Ѵ�.

		for (int i=0; i<3; i++) {
			// s_det�� ����ϴ� ����, �ӵ��׷����� ���� �������� �Ʒ��� ���������� �Ǻ��ϱ� ���ؼ���.
			float v_0_2 = v_0*v_0;
			float s_det = (m_pi/2.0f)*0.5f*Sign_f(v_0)*v_0_2/a2;
			int   sign  = (s_det > S) ? -1 : 1;

			// v_p2�� �׻� ���� ���� �Ǿ�� �Ѵ�.
			float v_p2 = (v_0_2*a2 + sign*4*a1*a2*S/m_pi)/(a1 + a2);
			if (v_p2 > EPS) {
				v_x = sign*sqrt(v_p2);

				// v_x�� v_max�� �ʰ��� �� ���⶧���� ������ ���� v_x�� �����Ѵ�.
				if      (v_x >  v_max) v_x =  v_max;
				else if (v_x < -v_max) v_x = -v_max;
			}
			else {
				v_x = v_0;
				if (v_0 && S) {
					a1 = a2 = (m_pi/2)*0.5f*fabs(v_0_2/S);
				}
				float j1 = (T1 > EPS) ? (m_pi*m_pi)*fabs(S1a/(T1*T1*T1)) : J;
				float j2 = (T2 > EPS) ? (m_pi*m_pi)*fabs(S2 /(T2*T2*T2)) : J;

				if      (j1 > J*2          ) { a1 *= sqrt(sqrt(J/j1 + EPS)); }
				else if (j1 < J/2 && a1 < a) { a1 *= sqrt(sqrt(J/j1 + EPS)); }

				if      (j2 > J*2          ) { a2 *= sqrt(sqrt(J/j2 + EPS)); }
				else if (j2 < J/2 && a2 < d) { a2 *= sqrt(sqrt(J/j2 + EPS)); }
			}

			// ���ӱ���(T1, S1)�� ���ӱ���(T2, S2), ���ӱ���(T3, S3)�� ���� �ð��� �̵��Ÿ��� ����Ѵ�.
			float dv = v_x - v_0;
			T1  = max(dt, (m_pi/2)*fabs(dv/a1));
			T2  = max(dt, (m_pi/2)*fabs(v_x/a2));
			S1a = dv*T1/2;
			S1b = v_0*T1;
			S2  = v_x*T2/2;

			// �ִ� ��ũ�� ũ�⸦ ����� ����.
			// ���� ���������� ũ�ٸ�, �ִ� ���ӵ� ũ�⸦ ���߾� ���� ����Ѵ�.
			bool  repeat = false;
			float j1 = (T1 > EPS) ? (m_pi*m_pi)*fabs(S1a/(T1*T1*T1)) : J;
			float j2 = (T2 > EPS) ? (m_pi*m_pi)*fabs(S2 /(T2*T2*T2)) : J;

			if      (j1 > J*2          ) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }
			else if (j1 < J/2 && a1 < a) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }

			if      (j2 > J*2          ) { a2 *= sqrt(sqrt(J/j2 + EPS)); repeat = true; }
			else if (j2 < J/2 && a2 < d) { a2 *= sqrt(sqrt(J/j2 + EPS)); repeat = true; }

			if (!repeat) break;
		}

		S1 = S1a + S1b;
		T3 = (v_x) ? fabs((S - S1 - S2)/v_x) : 0;
		if (T3 < dt) T3 = 0;

		mode = 0;
		t = 0;
	}

	inline float position (float S, float T, float t) { return S*t/T - S/m_pi*sin(M_PI*t/T);	}
	inline float velocity (float S, float T, float t) { return S/T - S/T*cos(M_PI*t/T);		}
	inline float accelera (float S, float T, float t) { return S*m_pi/(T*T)*sin(M_PI*t/T); 	}

	bool VelocityProfile (float &v, float &a)
	{
		if (mode == 0) {
			if (t < T1) {
				a = accelera (S1a, T1, t);
				v = v_0 + velocity (S1a, T1, t);

				t += dt;
				return true;
			}
			else {
				mode = 1;
				t = 0;
				// go to next mode, do not return here.
			}
		}
		if (mode == 1) {
			a = 0;
			v = v_x;

			mode = -1;
			return true;
		}
		return false;
	}

	bool PositionProfile (int32_t &p, float &v, float &a)
	{
		if (mode == 0) {
			if (t < T1) {
				a = accelera (S1a, T1, t);
				v = v_0 + velocity (S1a, T1, t);
				p = p_0 + Round_f((v_0*t + position (S1a, T1, t))*rad2pulse);

				t += dt;
				return true;
			}
			else {
				mode = 1;
				t = t - T1;
				// go to next mode, do not return here.
			}
		}
		if (mode == 1) {
			if (t < T3) {
				a = 0;
				v = v_x;
				p = p_0 + Round_f((S1 + v_x*t)*rad2pulse);

				t += dt;
				return true;
			}
			else {
				mode = 2;
				t = T2 - (t - T3);
				// go to next mode, do not return here.
			}
		}
		if (mode == 2) {
			if (t > 0) {
				a = -accelera (S2, T2, t);
				v = velocity (S2, T2, t);
				p = p_e - Round_f(position (S2, T2, t)*rad2pulse);

				t -= dt;
				return true;
			}
			else {
				a = 0;
				v = 0;
				p = p_e;

				mode = -1;
				return true;
			}
		}
		return false;
	}
};
*/

class SinusoidalProfile {
private:
	float dt;
	float pulse2rad;
	float rad2pulse;

	int32_t  p_0;	// ���� ��ġ
	int32_t  p_e;		// �� ��ġ
	float v_0;		// �ʱ� �ӵ�
	float v_x;		// �ְ� �ӵ�(��ġ ��������), ���� �ӵ�(�ӵ� ��������)

	float S, S1a, S1b, S1, S2;
	float T1, T2, T3;

	int32_t   mode;		// ���������� 3�ܰ� ���(0-����, 1-����, 2-����, -1-�������� ����)
	float t;		// ���������� ����� �ð�

public:
	SinusoidalProfile (int32_t f_) : dt(1.f/f_), mode(-1), t(0) { }

	void Reset ()
	{
		mode = -1;
	}

	void SetParam (float pulse2rad_, float rad2pulse_, float m_pi_)
	{
		pulse2rad = pulse2rad_;
		rad2pulse = rad2pulse_;
	}

	void InitVelocityProfile (float v_tgt, float v_ini, float v_max, float a, float d, float J)
	{
		assert (J     > EPS);
		assert (a     > EPS);
		assert (d     > EPS);
		assert (v_max > EPS);

		v_0 = v_ini;
		v_x = Limit_f (v_tgt, -v_max, v_max);

		float a0 = (fabs(v_x) > fabs(v_0)) ? a : d;
		float a1 = a0;

		for (int i=0; i<3; i++) {
			float dv = v_x - v_0;
			T1  = max(dt, (M_PI/2.0f)*fabs(dv/a1));
			S1a = 0.5f*dv*T1;
			S1b = v_0*T1;

			S1  = S1a + S1b;
			T2  = T3 = 0;

			bool  repeat = false;
			float j1 = (T1 > EPS) ? fabs(M_PI*M_PI*S1a/(T1*T1*T1)) : J;

			if      (j1 > J*2          ) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }
			else if (j1 < J/2 && a1 < a0) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }

			if (!repeat) break;
		}

		mode = 0;
		t = 0;
	}

	void InitPositionProfile (long p_del, long p_ini, float v_ini, float v_max, float a, float d, float J)
	{
		assert (J     > EPS);
		assert (a     > EPS);
		assert (d     > EPS);
		assert (v_max > EPS);

		v_0 = v_ini;
		p_0	= p_ini;
		p_e = p_ini + p_del;
		S	= p_del*pulse2rad;

		float a1 = a;
		float a2 = d;

		// ���� �ӵ� v_0���� ���ӵ� a_max�� �����Ͽ� ������ ���, �̵��Ÿ� S�� ������� üũ�Ѵ�.
		// ���� �ְ�ӵ��� ����ԵǸ�, v_x ���� �ְ� �ӵ��� �ΰ� �ٸ� �Ķ���͸� �����ؾ��Ѵ�.

		for (int i=0; i<3; i++) {
			// s_det�� ����ϴ� ����, �ӵ��׷����� ���� �������� �Ʒ��� ���������� �Ǻ��ϱ� ���ؼ���.
			float v_0_2 = v_0*v_0;
			float s_det = (M_PI/2.0f)*0.5f*Sign_f(v_0)*v_0_2/a2;
			int   sign  = (s_det > S) ? -1 : 1;

			// v_p2�� �׻� ���� ���� �Ǿ�� �Ѵ�.
			float v_p2 = (v_0_2*a2 + sign*4*a1*a2*S/M_PI)/(a1 + a2);
			if (v_p2 > EPS) {
				v_x = sign*sqrt(v_p2);

				// v_x�� v_max�� �ʰ��� �� ���⶧���� ������ ���� v_x�� �����Ѵ�.
				if      (v_x >  v_max) v_x =  v_max;
				else if (v_x < -v_max) v_x = -v_max;
			}
			else {
				v_x = v_0;
				if (v_0 && S) {
					a1 = a2 = (M_PI/2)*0.5f*fabs(v_0_2/S);
				}
				float j1 = (T1 > EPS) ? (M_PI*M_PI)*fabs(S1a/(T1*T1*T1)) : J;
				float j2 = (T2 > EPS) ? (M_PI*M_PI)*fabs(S2 /(T2*T2*T2)) : J;

				if      (j1 > J*2          ) { a1 *= sqrt(sqrt(J/j1 + EPS)); }
				else if (j1 < J/2 && a1 < a) { a1 *= sqrt(sqrt(J/j1 + EPS)); }

				if      (j2 > J*2          ) { a2 *= sqrt(sqrt(J/j2 + EPS)); }
				else if (j2 < J/2 && a2 < d) { a2 *= sqrt(sqrt(J/j2 + EPS)); }
			}

			// ���ӱ���(T1, S1)�� ���ӱ���(T2, S2), ���ӱ���(T3, S3)�� ���� �ð��� �̵��Ÿ��� ����Ѵ�.
			float dv = v_x - v_0;
			T1  = max(dt, (M_PI/2)*fabs(dv/a1));
			T2  = max(dt, (M_PI/2)*fabs(v_x/a2));
			S1a = dv*T1/2;
			S1b = v_0*T1;
			S2  = v_x*T2/2;

			// �ִ� ��ũ�� ũ�⸦ ����� ����.
			// ���� ���������� ũ�ٸ�, �ִ� ���ӵ� ũ�⸦ ���߾� ���� ����Ѵ�.
			bool  repeat = false;
			float j1 = (T1 > EPS) ? (M_PI*M_PI)*fabs(S1a/(T1*T1*T1)) : J;
			float j2 = (T2 > EPS) ? (M_PI*M_PI)*fabs(S2 /(T2*T2*T2)) : J;

			if      (j1 > J*2          ) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }
			else if (j1 < J/2 && a1 < a) { a1 *= sqrt(sqrt(J/j1 + EPS)); repeat = true; }

			if      (j2 > J*2          ) { a2 *= sqrt(sqrt(J/j2 + EPS)); repeat = true; }
			else if (j2 < J/2 && a2 < d) { a2 *= sqrt(sqrt(J/j2 + EPS)); repeat = true; }

			if (!repeat) break;
		}

		S1 = S1a + S1b;
		T3 = (v_x) ? fabs((S - S1 - S2)/v_x) : 0;
		if (T3 < dt) T3 = 0;

		mode = 0;
		t = 0;
	}

	inline float position (float S, float T, float t) { return S*t/T - S/M_PI*sin(M_PI*t/T);	}
	inline float velocity (float S, float T, float t) { return S/T - S/T*cos(M_PI*t/T);		}
	inline float accelera (float S, float T, float t) { return S*M_PI/(T*T)*sin(M_PI*t/T); 	}

	bool VelocityProfile (float &v, float &a)
	{
		if (mode == 0) {
			if (t < T1) {
				a = accelera (S1a, T1, t);
				v = v_0 + velocity (S1a, T1, t);

				t += dt;
				return true;
			}
			else {
				mode = 1;
				t = 0;
				// go to next mode, do not return here.
			}
		}
		if (mode == 1) {
			a = 0;
			v = v_x;

			mode = -1;
			return true;
		}
		return false;
	}

	bool PositionProfile (int32_t &p, float &v, float &a)
	{
		if (mode == 0) {
			if (t < T1) {
				a = accelera (S1a, T1, t);
				v = v_0 + velocity (S1a, T1, t);
				p = p_0 + Round_f((v_0*t + position (S1a, T1, t))*rad2pulse);

				t += dt;
				return true;
			}
			else {
				mode = 1;
				t = t - T1;
				// go to next mode, do not return here.
			}
		}
		if (mode == 1) {
			if (t < T3) {
				a = 0;
				v = v_x;
				p = p_0 + Round_f((S1 + v_x*t)*rad2pulse);

				t += dt;
				return true;
			}
			else {
				mode = 2;
				t = T2 - (t - T3);
				// go to next mode, do not return here.
			}
		}
		if (mode == 2) {
			if (t > 0) {
				a = -accelera (S2, T2, t);
				v = velocity (S2, T2, t);
				p = p_e - Round_f(position (S2, T2, t)*rad2pulse);

				t -= dt;
				return true;
			}
			else {
				a = 0;
				v = 0;
				p = p_e;

				mode = -1;
				return true;
			}
		}
		return false;
	}
};
