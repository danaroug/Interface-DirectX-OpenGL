#pragma once

class IRenderer
{
public:
	IRenderer();// Destractor
	virtual ~IRenderer(); //Virtual destractor
	
	//Pure virtual functions
	virtual int Run() = 0;
	virtual bool Init() = 0;

	/*virtual bool Init() = 0;
	virtual void Init_Graphics() = 0;
	virtual bool Display() = 0;*/

	/*virtual void Resize() = 0;
	virtual void Motion() = 0;
	virtual void Mouse() = 0;*/

};


