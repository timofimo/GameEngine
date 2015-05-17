#include "GameObject.h"


GameObject::GameObject(const std::string name)
{
	m_name = name;
}


GameObject::~GameObject()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if(m_components.at(i)->getComponentType() != GameComponent::RENDER) delete m_components.at(i);
	}
	m_components.clear();

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		delete m_children.at(i);
	}
	m_children.clear();
}

/*bool GameObject::operator==(GameObject other)
{
	return m_name == other.getName();
}

GameObject& GameObject::operator=(const GameObject& other)
{
	*this = GameObject(other);
	return *this;
}*/

void GameObject::start()
{
	// start all components
	for each (GameComponent* component in m_components)
	{
		component->start();
	}

	// start the children
	for each (GameObject* object in m_children)
	{
		object->start();
	}
}

void GameObject::update(float deltaTime)
{
	// update all components
	for each (GameComponent* component in m_components)
	{
		if (component->getComponentType() == GameComponent::SCRIPT)
			component->update(deltaTime);
	}

	// update the children
	for each (GameObject* object in m_children)
	{
		object->update(deltaTime);
	}
}

void GameObject::render(Shader* shader)
{
	// render object
	for each (GameComponent* component in m_components)
	{
		if (component->getComponentType() == GameComponent::RENDER)
			component->render(shader);
	}

	for each (GameObject* object in m_children)
	{
		object->render(shader);
	}
}

void GameObject::setParent(GameObject* parent)
{
	m_parent = parent;
}

void GameObject::addChild(GameObject* child)
{
	if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
		std::cout << m_name.c_str() << " already contains object " << child->getName().c_str() << std::endl;
	else
	{
		child->setParent(this);
		m_children.push_back(child);
	}
}

void GameObject::removeChild(const std::string name)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->getName() == name)
		{
			m_children.erase(m_children.begin() + i);
			return;
		}
	}
}

void GameObject::addComponent(GameComponent* component)
{
	for each (GameComponent* c in m_components)
	{
		if (c == component)
		{
			std::cout << m_name.c_str() << " already contains component " << component->getName().c_str() << std::endl;
			return;
		}
	}

	component->setParent(this);
	m_components.push_back(component);
}

void GameObject::removeComponent(const std::string name)
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->getName() == name)
		{
			m_components.erase(m_components.begin() + i);
			return;
		}
	}
}

Transform& GameObject::getLocalTransform()
{
	return m_localTransform;
}

Transform& GameObject::getWorldTransform()
{
	if (m_parent)
	{
		if (transformChanged())
		{
			Transform t = m_localTransform;
			Transform pt(m_parent->getWorldTransform());

			t.position() *= pt.scale();
			t.scale() *= pt.scale();
			t.position() = pt.position() + (pt.rotationQ() * t.position());
			t.rotate(pt.rotation());

			return m_worldTransform = t;
		}
		else
		{
			m_worldTransform.setHasChanged(false);
			return m_worldTransform;
		}
	}
	else return m_localTransform;
}

bool GameObject::transformChanged()
{
	bool result = m_localTransform.hasChanged();
	if (m_parent)
		result |= m_parent->transformChanged();
	return result;
}

std::string GameObject::getName()
{
	return m_name;
}

