#include <mono/jit/jit.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <stdlib.h>

#include "globals.h"
#include "mono_hooks.h"


MonoDomain *domain;
MonoAssembly *assembly;


void mono_call()
{
    MonoImage *image = mono_assembly_get_image(assembly);
    MonoClass *klass = mono_class_from_name(image, G_paths.mono_namespace, G_paths.mono_class);


    void *params[1] = { NULL };
    MonoMethod *method = mono_class_get_method_from_name(klass, G_paths.mono_callback, 0);
    mono_runtime_invoke(method, NULL, params, NULL);
}


void init_mono()
{
    mono_config_parse(NULL);
    
    domain = mono_jit_init_version("mono_interop", "v4.0.30319");
    assembly = mono_domain_assembly_open(domain, G_paths.mono_interop_exe);
    
    //mono_add_internal_call ("mono_interop.InteropMain::DoPacket", c_callback);

    // call the "int static main(string[] args)" with useless info, actual info is in the global struct
    char *argv[2]= {"nope.avi","foo.bar"};
    mono_jit_exec (domain, assembly, 2, argv);

    mono_call();
}