#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <unordered_map>
#include <typeindex>


#include <iostream>

constexpr uint32_t MAX_ENTITIES = 2048;
constexpr uint32_t MAX_COMPONENTS = 1024;
constexpr uint32_t NULL_INDEX = -1;

using Entity = uint32_t;

// ENTITY MANAGER =======================================================================
class EntityManager {
private:
	// Sparse array. entity == packed[sparse[entity]]
	std::array<Entity, MAX_ENTITIES> sparse = { NULL_INDEX };

	// Packed array de entidades activas para favorecer iteracion sin tener que comprobar todas
	std::array<Entity, MAX_ENTITIES> packed;

	// Array de indices libres
	std::array<Entity, MAX_ENTITIES> freeIndices;
	uint32_t freeCount = MAX_ENTITIES;
	uint32_t count = 0;

public:

	EntityManager() {
		for (Entity i = 0; i < MAX_ENTITIES; i++) {
			freeIndices[i] = MAX_ENTITIES - i - 1;
		}
	}

	Entity createEntity() {
		if (freeCount == 0) return NULL_INDEX;

		// Coger el siguiente id libre
		freeCount--;
		Entity newId = freeIndices[freeCount];

		// Insertar
		sparse[newId] = count;
		packed[count] = newId;

		count++;

		return newId;
	}

	void destroyEntity(Entity entity) {
		if (sparse[entity] == NULL_INDEX) return;

		// eliminar todos sus componentes asociados


		// cambiar ultimo por eliminado para mantener packed
		uint32_t index = sparse[entity];
		Entity lastEntity = packed[count - 1];

		packed[index] = lastEntity;
		sparse[lastEntity] = index;

		// Liberar entidad
		sparse[entity] = NULL_INDEX;
		
		freeCount++;
		freeIndices[freeCount] = entity;
		count--;
	}

	uint32_t GetCount() const { return count; }

	const std::array<Entity, MAX_ENTITIES>& GetEntities() const { return packed; }


};


// COMPONENT ARRAY BASE =======================================================================
class ComponentArrayBase {
public:
	virtual void Remove(Entity entity) = 0;
	virtual bool Has(Entity entity) = 0;
};

// COMPONENT ARRAY =======================================================================
template <typename T>
class ComponentArray: public ComponentArrayBase{
private:

	// Packed array de componentes
	std::array<T, MAX_ENTITIES> components;

	// Sparse array de entidades 
	// sparse[entidad] -> indice del componente en packed
	std::array<uint32_t, MAX_ENTITIES> sparse = { NULL_INDEX };

	// Packed array de entidades asociadas a cada componente
	// entities[indice] -> entidad que posee components[indice]
	std::array<Entity, MAX_ENTITIES> entities;

	// Índices libres y contador
	std::array<uint32_t, MAX_ENTITIES> freeIndices;
	uint32_t freeCount = MAX_ENTITIES;
	uint32_t count = 0;

public:

	ComponentArray() {
		// Inicializar lista de indices libres
		for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
			// Rellenamos de mayor a menor
			// freeIndices[MAX_ENTITIES - 1] -> 0
			freeIndices[i] = MAX_ENTITIES - i - 1;
			sparse[i] = NULL_INDEX;
		}
		freeCount = MAX_ENTITIES;
	}

	void Add(Entity entity, T component) {
		// Si la entidad ya está en el set -> no se hace nada
		if (sparse[entity] != NULL_INDEX) {
			std::cerr << "ERROR: Entity " << entity << " already has this component!" << std::endl;
			return;
		}

		if (freeCount == 0) {
			std::cerr << "ERROR: ComponentArray full for type " << typeid(T).name() << std::endl;
			return;
		}

		// Nuevo indice para los packed arrays
		freeCount--;
		uint32_t newIndex = freeIndices[freeCount];

		// Introducir datos
		sparse[entity] = newIndex;
		entities[newIndex] = entity;
		components[newIndex] = component;
		count++;

	}

	void Remove(Entity entity) {
		uint32_t index = sparse[entity];

		//Si la entidad no está en el set -> no se hace nada
		if (index == NULL_INDEX) return;


		// Movemos el ultimo elemento a la posicion que queremos borrar
		// Así el array sigue siendo compacto

		uint32_t lastIndex = count - 1;
		if (index != lastIndex) {
			Entity lastEntity = entities[lastIndex];

			components[index] = components[lastIndex];
		}

		// Marcar entidad borrada como vacia
		sparse[entity] = NULL_INDEX;

		// Añadir lastIndex a los indices libres
		freeIndices[freeCount] = lastIndex;
		freeCount++;
		count--;
		
	}

	T& Get(Entity entity) {
		return components[sparse[entity]];
	}

	bool Has(Entity e) {
		return e < MAX_ENTITIES &&
			sparse[e] < count &&
			sparse[e] != NULL_INDEX;
	}

	// Iteradores begin y end para iterar por los componentes (range based loops)
	T* begin() { return &components[0]; }
	T* end() { return &components[count]; } // puntero que apunta al siguiente del ultimo

	const std::array<Entity, MAX_ENTITIES>& GetEntities() const { return entities; }
	uint32_t GetCount() const { return count; }

};

// COMPONENT MANAGER =======================================================================
class ComponentManager {
private:
	// Array de ComponentArray
	// se usa un puntero a void para poder apuntar a arrays de distintos tipos
	std::array<std::shared_ptr<ComponentArrayBase>, MAX_COMPONENTS> componentArrays;

	// Mapa de los tipos de componentes
	// type_index guarda el nombre de los tipos
	// El mapa asocia un tipo de datos (tipo de componente) a un indice
	//std::unordered_map<std::type_index, uint32_t> componentTypes;

	uint32_t nextTypeId = 0;

	template<typename T>
	uint32_t GetTypeId() {
		// La funcion es un template y typeid es static ->
		// type id se asigna una unica vez por tipo
		static uint32_t typeId = nextTypeId;
		if(typeId == nextTypeId) nextTypeId++;

		return typeId;
	}

public:
	template<typename T>
	void RegisterComponent() {
		uint32_t typeId = GetTypeId<T>();

		// hacer que el puntero apunte al nuevo array de componentes de tipo T
		componentArrays[typeId] = std::make_shared<ComponentArray<T>>();

		// Guardamos la info del tipoT
		// Insertamos el id asociandolo a su tipo en el mapa
		//componentTypes[std::type_index(typeid(T))] = typeId;
	}

	template<typename T>
	ComponentArray<T>& GetArray() {
		// Cogemos de los arrays de componentes el array de tipo T 
		// Como componentArrays es un array de shared pointers utilizamos .get() 
		//		para obtener el puntero a ComponentArraybase
		// Hacemos el casteo de void ptr a ComponentArray<T>*
		// * -> desreferenciamos
		return *static_cast<ComponentArray<T>*>(componentArrays[GetTypeId<T>()].get());
	}

	template<typename T>
	void AddComponent(Entity entity, T component) {
		GetArray<T>().Add(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		GetArray<T>().Remove(entity);
	}

	void RemoveAllComponents(Entity entity) {
		for (unsigned int i = 0; i < nextTypeId; i++) {
			if (componentArrays[i] != 0) {
				componentArrays[i]->Remove(entity);
			}
		}
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		return GetArray<T>().Get(entity);
	}

	template<typename T>
	bool HasComponent(Entity entity) {
		return GetArray<T>().Has(entity);
	}

};
