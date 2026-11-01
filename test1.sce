name CALC{
    obj calc_pi(){
        obj pi = 0.0;
		obj sign = 1.0;
		obj denominator = 1.0;
		obj i = 0;
		obj u_sign = -1.0;
		while (i < 1000000) {
			pi = pi + sign / denominator;
			sign = sign * u_sign;
			denominator = denominator + 2.0;
			i++;
		}
		pi = pi * 4.0;
        return pi;
    }
	obj calc_e(){
		obj e = 1.0;
		obj factorial = 1.0;
		obj i = 1;
		while (i < 1000000) {

			factorial = factorial * i;

			e = e + 1.0 / factorial;

			i++;
		}
		return e;
	}
	obj calc_sqrt(obj a) {
		obj x = a;
		obj i = 0;
		while (i < 20) {
			x = 0.5 * (x + a / x);
			i++;
		}
		return x;
	}
	obj calc_sin(obj x) {
		obj result = 0.0;
		obj sign = 1.0;
		obj power = x;
		obj factorial = 1.0;
		obj i = 0;
		while (i < 15) {
			if (i > 0) {
				power = power * x * x;
				factorial = factorial * (2 * i) * (2 * i + 1);
				sign = sign * -1.0;
			}
			result = result + sign * power / factorial;
			i++;
		}
		return result;
	}
	obj calc_cos(obj x) {
		obj result = 0.0;
		obj sign = 1.0;
		obj power = 1.0;
		obj factorial = 1.0;

		obj i = 0;

		while (i < 15) {
			if (i > 0) {
				power = power * x * x;
				factorial = factorial * (2 * i - 1) * (2 * i);
				sign = sign * -1.0;
			}

			result = result + sign * power / factorial;

			i++;
		}

		return result;
	}
}
