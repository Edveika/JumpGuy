#include "XAudio2.h"

XAudio2::XAudio2()
{
	if (!create_xaudio2_device())
	{
		MessageBoxA(NULL, "[XAUDIO2]: Failed to create XAudio2 device!", NULL, NULL);
		throw std::invalid_argument("[XAUDIO2]: XAudio2 device not created");
	}
	if (!create_xaudio2_mastering_voice_obj())
	{
		MessageBoxA(NULL, "[XAUDIO2]: Failed to create a voice!", NULL, NULL);
		throw std::invalid_argument("[XAUDIO2]: Voice not created");
	}
}

XAudio2::~XAudio2()
{
	delete m_xaudio2;
	delete m_master_voice;
}

bool XAudio2::create_xaudio2_device()
{
	if (FAILED(XAudio2Create(&m_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) 
		return false;
	return true;
}

bool XAudio2::create_xaudio2_mastering_voice_obj()
{
	if (FAILED(m_xaudio2->CreateMasteringVoice(&m_master_voice))) 
		return false;
	return true;
}

HRESULT XAudio2::find_chunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD chunk_type;
	DWORD chunk_data_size;
	DWORD riff_data_size = 0;
	DWORD file_type;
	DWORD bytes_read = 0;
	DWORD offset = 0;

	while (hr == S_OK)
	{
		DWORD read;
		if (0 == ReadFile(hFile, &chunk_type, sizeof(DWORD), &read, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		
		if (0 == ReadFile(hFile, &chunk_data_size, sizeof(DWORD), &read, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (chunk_type)
		{
		case fourccRIFF:
			riff_data_size = chunk_data_size;
			chunk_data_size = 4;
			if (0 == ReadFile(hFile, &file_type, sizeof(DWORD), &read, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, chunk_data_size, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		offset += sizeof(DWORD) * 2;

		if (chunk_type == fourcc)
		{
			dwChunkSize = chunk_data_size;
			dwChunkDataPosition = offset;
			return S_OK;
		}

		offset += chunk_data_size;

		if (bytes_read >= riff_data_size) 
			return S_FALSE;
	}

	return S_OK;
}

HRESULT XAudio2::read_chunk_data(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD read;
	if (0 == ReadFile(hFile, buffer, buffersize, &read, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());

	return hr;
}

IXAudio2SourceVoice* XAudio2::load_audio_data(LPCWSTR fileName)
{
	HRESULT hr;
	IXAudio2SourceVoice* tmp;

	// Open the file
	HANDLE file = CreateFile
	(
		fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (INVALID_HANDLE_VALUE == file) 
		return NULL;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(file, 0, NULL, FILE_BEGIN)) 
		return NULL;

	//check the file type, should be fourccWAVE or 'XWMA'
	find_chunk(file, fourccRIFF, m_chunk_size, m_chunk_position);
	DWORD file_type;
	read_chunk_data(file, &file_type, sizeof(DWORD), m_chunk_position);

	if (file_type != fourccWAVE)
		return NULL;

	find_chunk(file, fourccFMT, m_chunk_size, m_chunk_position);
	read_chunk_data(file, &m_wfx, m_chunk_size, m_chunk_position);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	find_chunk(file, fourccDATA, m_chunk_size, m_chunk_position);
	BYTE* data_buffer = new BYTE[m_chunk_size];
	read_chunk_data(file, data_buffer, m_chunk_size, m_chunk_position);

	m_buffer.AudioBytes = m_chunk_size;  //size of the audio buffer in bytes
	m_buffer.pAudioData = data_buffer;  //buffer containing audio data
	m_buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	if (FAILED(hr = m_xaudio2->CreateSourceVoice(&tmp, (WAVEFORMATEX*)&m_wfx))) 
		return NULL;

	if (FAILED(hr = tmp->SubmitSourceBuffer(&m_buffer))) 
		return NULL;

	return tmp;
}

bool XAudio2::play_audio(IXAudio2SourceVoice* pSourceVoice)
{
	if (FAILED(pSourceVoice->Start(0))) 
		return false;
	return true;
}

bool XAudio2::stop_audio(IXAudio2SourceVoice* pSourceVoice)
{
	if (FAILED(pSourceVoice->Stop(0)))
		return false;
	return false;
}