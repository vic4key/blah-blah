#include <rttidata.h>

auto pObject = new class instance;

auto pBase = PBYTE(module);

auto address_vtable = *reinterpret_cast<ULONG_PTR*>(pObject);
address_vtable -= sizeof(ULONG_PTR);
auto address_rtti = *(ULONG_PTR*)(address_vtable);

auto ptr_rtti = reinterpret_cast<_RTTICompleteObjectLocator*>(address_rtti);
assert(ptr_rtti != nullptr);

auto pTypeDescriptor  = reinterpret_cast<TypeDescriptor*>(pBase + ptr_rtti->pTypeDescriptor);
auto pClassDescriptor = reinterpret_cast<_RTTIClassHierarchyDescriptor*>(pBase + ptr_rtti->pClassDescriptor);

auto pBaseClassArray = reinterpret_cast<DWORD*>(pBase + pClassDescriptor->pBaseClassArray);
assert(pBaseClassArray != nullptr);

for (auto i = 0u; i < pClassDescriptor->numBaseClasses; i++)
{
	auto pBaseClass = reinterpret_cast<_RTTIBaseClassDescriptor*>(pBase + pBaseClassArray[i]);
	assert(pBaseClass != nullptr);

	auto pClassDescriptor = reinterpret_cast<_RTTIClassHierarchyDescriptor*>(pBase + pBaseClass->pClassDescriptor);
	auto pTypeDescriptor = reinterpret_cast<TypeDescriptor*>(pBase + pBaseClass->pTypeDescriptor);

	if (pTypeDescriptor->name != nullptr)
	{
		auto class_name = Utils::undecorate_cpp_name(pTypeDescriptor->name);

		vu::msg_debug_A("%p %s `%s`\n", pTypeDescriptor->pVFTable, pTypeDescriptor->name, class_name.c_str());

		// if (vu::contains_string_A(class_name, pobject->name))
		// {
		//   // list all methods in the current base class
		//   // then traversal to its base classes
		//   // and append all their methods to method list
	}
}