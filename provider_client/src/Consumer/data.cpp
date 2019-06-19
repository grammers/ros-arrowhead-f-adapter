#include <iostream>

class Data{
public:
	static int getTemp(){
		return temperature;
	}
	static void setTemp(int temp){
		temperature = temp;
	}
private:
	static int temperature;
};
