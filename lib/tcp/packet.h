#pragma once

struct PHead
{
    unsigned int m_type;
    unsigned int m_size;
	char m_data[];
	PHead()
	{
		m_type = 0;
		m_size = 0;
	}
};