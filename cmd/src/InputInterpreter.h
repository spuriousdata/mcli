#ifndef __INPUTINTERPRETER_H__
#define __INPUTINTERPRETER_H__

class MemcacheClient;

class InputInterpreter
{
	public:
		InputInterpreter(MemcacheClient *mc) : mc(mc) {}
		void interpret(const char *input);

	private:
		MemcacheClient *mc;
};

#endif
