#pragma once
#include <xhash>

namespace Hazel
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return this->m_UUID; }

	private:
		uint64_t m_UUID;
	};
}
//���unordered_map����ʶ���Զ����������
namespace std
{
	template<>
	struct hash<Hazel::UUID>
	{
		std::size_t operator()(const Hazel::UUID& uuid)
		{
			return std::hash<uint64_t>()(uuid);
		}
	};
}
