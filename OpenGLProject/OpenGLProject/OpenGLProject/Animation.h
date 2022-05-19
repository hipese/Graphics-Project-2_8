#pragma once
#include "Model.h"

class Animation
{
private:
	// �ִϸ��̼ǿ� ���Ե� ���� �迭�Դϴ�.
	Model** m_models = nullptr;
	// �ִϸ��̼��� �ִ� ������ �� �Դϴ�.
	int m_max_frame = 0;
	// �ִϸ��̼� ������ �� ���� �������Դϴ�.
	int m_current_frame = 0;
	// �ִϸ��̼��� �ݺ� �����Դϴ�.
	bool m_bLoop = false;
	// �ִϸ��̼��� ��� �����Դϴ�.
	int m_Playing = 0;
public:
	inline Model** GetModels() { return m_models; }
	// �ִϸ��̼��� �ݺ��Ϸ��� true�� �����ϼ���.
	inline void SetbLoop(int bLoop) { m_bLoop = bLoop; }
	// �ִϸ��̼��� ����մϴ�.
	inline void Play() { m_Playing = 1; }
	// �ִϸ��̼��� �����մϴ�.
	inline void Stop() { m_Playing = 0; m_current_frame = 0; }
	// �ִϸ��̼��� �Ͻ������մϴ�.
	inline void Parse() { m_Playing = 0; }
	// �ִϸ��̼��� ������մϴ�.
	inline void ReversePlay() { m_Playing = -1; }
	inline bool IsEndFrame() { return m_current_frame == m_max_frame - 1; }
public:
	Animation();
	virtual ~Animation();

	// Model�� ���Ϸκ��� �ҷ��ɴϴ�.
	void LoadModel(const char* files, int max_frame, Transform* parent = nullptr);
	// �������� �ʱ�ȭ�մϴ�.
	void InitFrame();
	// ���� ���������� �����մϴ�.
	void PrevFrame();
	// ���� ���������� �����մϴ�.
	void NextFrame();
	// ���� �������� ���� �׸��ϴ�.
	void Draw(class Shader& shader);
	// ������Ʈ ȣ�� �� ����ϼ���.
	void Update();

};