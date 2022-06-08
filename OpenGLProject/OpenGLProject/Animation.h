#pragma once
#include "Model.h"

class Animation
{
private:
	// �ִϸ��̼ǿ� ���Ե� ���� �迭�Դϴ�.
	Model** m_models = nullptr;
	// �ִϸ��̼��� �ִ� ������ �� �Դϴ�.
	int m_maxFrame = 0;
	// �ִϸ��̼� ������ �� ���� �������Դϴ�.
	int m_currentFrame = 0;
	// �ִϸ��̼��� �ݺ� �����Դϴ�.
	bool m_bLoop = false;
	// �ִϸ��̼��� ��� �����Դϴ�.
	int m_playing = 0;
	// �ִϸ��̼��� ��� ������ �����մϴ�.
	bool m_bIsReverse = false;
	// �ִϸ��̼��� ���� �� �������Դϴ�.
	int m_delay = 0;
	int m_currentDelay = 0;
	// �ִϸ��̼��� ���� �����Դϴ�.
	bool m_bEndPlay = false;

public:
	Animation();
	virtual ~Animation();

	inline Model** GetModels() { return m_models; }
	inline Model* GetModel() { return m_models[m_currentFrame]; }
	// �ִϸ��̼��� �ݺ��Ϸ��� true�� �����ϼ���.
	inline void SetbLoop(int bLoop) { m_bLoop = bLoop; }
	// �ִϸ��̼��� ����մϴ�.
	inline void Play() { m_playing = 1; InitFrame(false); }
	// �ִϸ��̼��� ������մϴ�.
	inline void ReversePlay() { m_playing = -1;  InitFrame(true); }
	// �ִϸ��̼��� �����մϴ�.
	inline void Stop() { m_playing = 0; InitFrame(m_bIsReverse); }
	// �ִϸ��̼��� �Ͻ������մϴ�.
	inline void Parse() { m_playing = 0; }
	inline bool IsReverse() { return m_bIsReverse; }
	inline bool IsEndPlay() { return m_bEndPlay; }
	inline bool IsPlaying() { return m_playing != 0; }
	inline int GetPlaying() { return m_playing; }
	inline int GetMaxFrame() { return m_maxFrame; }
	inline void SetDelay(int delay) { m_delay = delay; }

	// Model�� ���Ϸκ��� �ҷ��ɴϴ�.
	void LoadModel(const char* files, int maxFrame, Transform* parent = nullptr);
	// �������� �ʱ�ȭ�մϴ�.
	void InitFrame(bool bIsReverse = true);
	// ���� ���������� �����մϴ�.
	void PrevFrame();
	// ���� ���������� �����մϴ�.
	void NextFrame();
	// ���� �������� ���� �׸��ϴ�.
	void Draw(class Shader& shader);
	// ������Ʈ ȣ�� �� ����ϼ���.
	void Update();

};