#ifndef _EDITOR_H_
#define _EDITOR_H_

class Editor : public de::Application
{
public:
	void Initialize() override;

	void FixedUpdate() override;

private:
	de::Renderer m_Renderer;
};

#endif // !_EDITOR_H_
