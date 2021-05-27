import sys, win32api, pefile
from capstone import *
from PyVutils import Utils

def rva_to_offset(file_path, rva):
	pe = pefile.PE(file_path, fast_load=True)
	for section in pe.sections:
		if rva >= section.VirtualAddress:
			return section.PointerToRawData + rva - section.VirtualAddress
	return -1;

def get_storage_version_from_file(file, offset):
	try:
		file.seek(offset)
		data = file.read(20)
		cs = Cs(CS_ARCH_X86, CS_MODE_64)
		for i in cs.disasm(data, 0):
			cmd = i.mnemonic.lower()
			if cmd in ["ret", "int3"]:
				return -1
			elif cmd == "mov":
				matches = Utils.RegEx(i.op_str, "eax,.?([x\d]+)")
				if matches and matches[0]: return int(matches[0][0], 16)
			elif cmd == "jmp":
				len = int(i.op_str, 16)
				offset += len
				return get_storage_version_from_file(file, offset)
	except: pass
	return -1

def main():
	if len(sys.argv) != 2: print("invalid argument")
	else:
		file_path = sys.argv[1]
		version = -1
		module = win32api.LoadLibraryEx(file_path, 0, 1) # DONT_RESOLVE_DLL_REFERENCES
		method = win32api.GetProcAddress(module, "?GetCurrentVersion@CStorage@@QEAA?AW4eVersion@1@XZ")
		rva = method - module
		offset = rva_to_offset(file_path, rva)
		with open(file_path, "rb") as file:
			version = get_storage_version_from_file(file, offset)
		print(F"CStorage => {'unknown' if version == -1 else version}")
	return 0

if __name__ == "__main__":
  try: sys.exit(main())
  except (Exception, KeyboardInterrupt): Others.LogException(sys.exc_info())