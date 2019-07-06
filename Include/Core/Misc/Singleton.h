#pragma once

template<class T>
class Singleton
{
private:
	static T* m_instance;

protected:
	Singleton(void) {}

public:
	static T* Instance(void) { return m_instance; }
	static void Init(void) 
	{
		//TODO : m_instance != nullpte Assert
		m_instance = new T();
		m_instance->OnInit();
	}
	static void Destroy(void)
	{
		if (m_instance != nullptr)
		{
			m_instance->OnDestroy();
			delete m_instance;
			m_instance = nullptr;
		}
	}

protected:
	virtual void OnInit() = 0;
	virtual void OnDestroy() = 0;
};