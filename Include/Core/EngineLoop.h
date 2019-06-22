#pragma once

/*
 *	继承该类，重写方法来进行自定义的逻辑和渲染
 *	工程中可存在多个该类的子类，例如实现清屏Demo、画三角Demo、灯光、阴影等等Demo时可以分别派生子类
 *	但是只允许存在一个入口Loop
 */

class EngineLoop
{
public:
	virtual void Init() = 0;
	virtual void Resize() { }
	virtual void Destroy() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};