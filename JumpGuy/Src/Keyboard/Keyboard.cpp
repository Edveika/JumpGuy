#include "Keyboard.h"

Keyboard::Keyboard(HINSTANCE hInstance)
{
	// Create direct input object
	if (!create_direct_input_object(hInstance))
	{
		MessageBoxA(NULL, "[KEYBOARD]: Failed to create Direct input object!", NULL, NULL);
		throw std::invalid_argument("[KEYBOARD]: Direct input obj not created");
	}

	// Create direct input device
	if (!create_direct_input_device())
	{
		MessageBoxA(NULL, "[KEYBOARD]: Failed to create Direct input device!", NULL, NULL);
		throw std::invalid_argument("[KEYBOARD]: Direct input device not created");
	}

	// Set direct input format
	if (!set_data_format())
	{
		MessageBoxA(NULL, "[KEYBOARD]: Failed to set data format!", NULL, NULL);
		throw std::invalid_argument("[KEYBOARD]: Data format not set");
	}

	// Acquire access
	if (!acquire_access())
	{
		MessageBoxA(NULL, "[KEYBOARD]: Failed to acquire access!", NULL, NULL);
		throw std::invalid_argument("[KEYBOARD]: Access not acquired");
	}
}

Keyboard::~Keyboard()
{
	m_direct_input->Release();
	m_direct_input_dev->Release();
}

void Keyboard::get_input()
{
	if (FAILED(m_direct_input_dev->GetDeviceState(sizeof(m_buffer), (LPVOID)&m_buffer)))
		MessageBoxA(NULL, "Failed to get input from a direct input device!", NULL, NULL);
}

bool Keyboard::is_pressed(int key)
{
	if (m_buffer[key] & 0x80) 
		return true;

	return false;
}

bool Keyboard::create_direct_input_object(HINSTANCE hInstance)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL)))
		return false;

	return true;
}

bool Keyboard::create_direct_input_device()
{
	if (FAILED(m_direct_input->CreateDevice(GUID_SysKeyboard, &m_direct_input_dev, NULL))) 
		return false;

	return true;
}

bool Keyboard::set_data_format()
{
	if (FAILED(m_direct_input_dev->SetDataFormat(&c_dfDIKeyboard))) 
		return false;

	return true;
}

bool Keyboard::acquire_access()
{
	HRESULT Aqcuire(VOID);

	if (FAILED(m_direct_input_dev->Acquire())) 
		return false;

	return true;
}