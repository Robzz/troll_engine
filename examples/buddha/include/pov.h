#ifndef POV_H
#define POV_H

#include "image.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class Viewpoint {
public:
	friend boost::serialization::access;
	Viewpoint(Engine::Image const& colorMap, Engine::Image const& depthMap, Engine::Image const& normalMap);
	~Viewpoint();

protected:
	Engine::Image m_colorMap;
	Engine::Image m_depthMap;
	Engine::Image m_normalMap;

private:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & m_colorMap & m_depthMap & m_normalMap;
	}
};

#endif // POV_H