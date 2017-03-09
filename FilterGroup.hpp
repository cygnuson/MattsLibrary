#pragma once

#include <deque>

#include "Filter.hpp"
#include "Logger.hpp"

namespace cg {

/**A Group of filters.*/
class FilterGroup : public cg::Filter
{
public:
	/**Virtual DTOR, clean up all the filters.*/
	virtual ~FilterGroup()
	{
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			delete m_filters[i];
	}
	/**Add a filter.
	\param filter The filter to add.*/
	inline void AddFilter(cg::Filter* filter)
	{
		m_filters.push_back(filter);
	}
	/**Create empty.*/
	FilterGroup() {};
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	inline virtual void Transform(char* data, std::size_t size)
	{
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			m_filters[i]->Transform(data, size);
	}
	/**Transform data using the transform copy method.
	\param data The data place.
	\param size The data size.*/
	inline virtual ArrayView TransformCopy(const char* data, std::size_t size)
	{
		auto av = ArrayView::Copy(data, size);
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			av = m_filters[i]->TransformCopy(av.data(), av.size());
		return av;
	}
private:
	/**A list of filters with bool. If second is true, the filter is expected
	to change the size of the data.*/
	std::deque<cg::Filter*> m_filters;
};

}