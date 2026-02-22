#include "ext.h"
#include "ext_obex.h"
#include "ext_assist.h"
#include "z_dsp.h"

#include <algorithm>
#include <cstring>

#include "ToTape6Engine.hpp"

typedef struct _t_airfx_totape6_tilde {
    t_pxobject obj;
    ToTape6Engine engine;
    double param_0;
    double param_1;
    double param_2;
    double param_3;
    double param_4;
    double param_5;
    double sample_rate;
} t_airfx_totape6_tilde;

static t_class* s_airfx_totape6_tilde_class = nullptr;
static constexpr std::size_t k_assist_max_len = ASSIST_MAX_STRING_LEN;
static constexpr long k_airfx_totape6_tilde_param_count = 6;

static double airfx_totape6_tilde_clamp01(double value)
{
    return std::max(0.0, std::min(1.0, value));
}

static bool airfx_totape6_tilde_has_attr_args(long argc, t_atom* argv)
{
    for (long i = 0; i < argc; ++i) {
        if (atom_gettype(argv + i) == A_SYM) {
            t_symbol* sym = atom_getsym(argv + i);
            if (sym && sym->s_name && sym->s_name[0] == '@') {
                return true;
            }
        }
    }
    return false;
}

static void airfx_totape6_tilde_set_parameter_by_index(t_airfx_totape6_tilde* x, long index, double value)
{
    if (!x || index < 0 || index >= k_airfx_totape6_tilde_param_count) {
        return;
    }

    const double clamped = airfx_totape6_tilde_clamp01(value);
    switch (index) {
        case 0: x->param_0 = clamped; break;
        case 1: x->param_1 = clamped; break;
        case 2: x->param_2 = clamped; break;
        case 3: x->param_3 = clamped; break;
        case 4: x->param_4 = clamped; break;
        case 5: x->param_5 = clamped; break;
        default: return;
    }
    x->engine.setParameterByIndex(static_cast<int>(index), clamped);
}

static void airfx_totape6_tilde_named_param_0(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 0, value);
}

static void airfx_totape6_tilde_named_param_1(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 1, value);
}

static void airfx_totape6_tilde_named_param_2(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 2, value);
}

static void airfx_totape6_tilde_named_param_3(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 3, value);
}

static void airfx_totape6_tilde_named_param_4(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 4, value);
}

static void airfx_totape6_tilde_named_param_5(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 5, value);
}

static t_max_err airfx_totape6_tilde_attr_set_0(t_airfx_totape6_tilde* x, void* /*attr*/, long argc, t_atom* argv)
{
    if (!x || argc < 1 || !argv) {
        return MAX_ERR_GENERIC;
    }
    airfx_totape6_tilde_set_parameter_by_index(x, 0, atom_getfloat(argv));
    return MAX_ERR_NONE;
}

static t_max_err airfx_totape6_tilde_attr_set_1(t_airfx_totape6_tilde* x, void* /*attr*/, long argc, t_atom* argv)
{
    if (!x || argc < 1 || !argv) {
        return MAX_ERR_GENERIC;
    }
    airfx_totape6_tilde_set_parameter_by_index(x, 1, atom_getfloat(argv));
    return MAX_ERR_NONE;
}

static t_max_err airfx_totape6_tilde_attr_set_2(t_airfx_totape6_tilde* x, void* /*attr*/, long argc, t_atom* argv)
{
    if (!x || argc < 1 || !argv) {
        return MAX_ERR_GENERIC;
    }
    airfx_totape6_tilde_set_parameter_by_index(x, 2, atom_getfloat(argv));
    return MAX_ERR_NONE;
}

static t_max_err airfx_totape6_tilde_attr_set_3(t_airfx_totape6_tilde* x, void* /*attr*/, long argc, t_atom* argv)
{
    if (!x || argc < 1 || !argv) {
        return MAX_ERR_GENERIC;
    }
    airfx_totape6_tilde_set_parameter_by_index(x, 3, atom_getfloat(argv));
    return MAX_ERR_NONE;
}

static t_max_err airfx_totape6_tilde_attr_set_4(t_airfx_totape6_tilde* x, void* /*attr*/, long argc, t_atom* argv)
{
    if (!x || argc < 1 || !argv) {
        return MAX_ERR_GENERIC;
    }
    airfx_totape6_tilde_set_parameter_by_index(x, 4, atom_getfloat(argv));
    return MAX_ERR_NONE;
}

static t_max_err airfx_totape6_tilde_attr_set_5(t_airfx_totape6_tilde* x, void* /*attr*/, long argc, t_atom* argv)
{
    if (!x || argc < 1 || !argv) {
        return MAX_ERR_GENERIC;
    }
    airfx_totape6_tilde_set_parameter_by_index(x, 5, atom_getfloat(argv));
    return MAX_ERR_NONE;
}

static void* airfx_totape6_tilde_new(t_symbol* /*s*/, long argc, t_atom* argv)
{
    auto* x = (t_airfx_totape6_tilde*)object_alloc(s_airfx_totape6_tilde_class);
    if (!x) {
        return nullptr;
    }

    dsp_setup((t_pxobject*)x, 2);
    outlet_new((t_object*)x, "signal");
    outlet_new((t_object*)x, "signal");

    x->sample_rate = sys_getsr();
    if (x->sample_rate <= 0.0) {
        x->sample_rate = 44100.0;
    }

    x->engine.reset(x->sample_rate);
    x->param_0 = x->engine.getParameterByIndex(0);
    x->param_1 = x->engine.getParameterByIndex(1);
    x->param_2 = x->engine.getParameterByIndex(2);
    x->param_3 = x->engine.getParameterByIndex(3);
    x->param_4 = x->engine.getParameterByIndex(4);
    x->param_5 = x->engine.getParameterByIndex(5);

    const bool has_attrs = airfx_totape6_tilde_has_attr_args(argc, argv);
    if (!has_attrs && argc > 0) {
        const long assign_count = std::min<long>(argc, k_airfx_totape6_tilde_param_count);
        for (long i = 0; i < assign_count; ++i) {
            const auto atom_type = atom_gettype(argv + i);
            if (atom_type != A_LONG && atom_type != A_FLOAT) {
                break;
            }
            airfx_totape6_tilde_set_parameter_by_index(x, i, atom_getfloat(argv + i));
        }
    }

    if (has_attrs) {
        attr_args_process(x, argc, argv);
    }

    return x;
}

static void airfx_totape6_tilde_free(t_airfx_totape6_tilde* x)
{
    dsp_free((t_pxobject*)x);
}

static void airfx_totape6_tilde_float(t_airfx_totape6_tilde* x, double value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 0, value);
}

static void airfx_totape6_tilde_int(t_airfx_totape6_tilde* x, long value)
{
    airfx_totape6_tilde_set_parameter_by_index(x, 0, static_cast<double>(value));
}

static void airfx_totape6_tilde_param(t_airfx_totape6_tilde* x, t_symbol* /*s*/, long argc, t_atom* argv)
{
    if (argc < 1) {
        return;
    }

    long index = 0;
    double value = atom_getfloat(argv);

    if (argc >= 2) {
        if (atom_gettype(argv) == A_SYM) {
            t_symbol* name = atom_getsym(argv);
            if (!name || !name->s_name) {
                return;
            }
            if (false) {}
                        else if (std::strcmp(name->s_name, "input") == 0) { index = 0; }
            else if (std::strcmp(name->s_name, "soften") == 0) { index = 1; }
            else if (std::strcmp(name->s_name, "head_b") == 0) { index = 2; }
            else if (std::strcmp(name->s_name, "flutter") == 0) { index = 3; }
            else if (std::strcmp(name->s_name, "output") == 0) { index = 4; }
            else if (std::strcmp(name->s_name, "dry_wet") == 0) { index = 5; }
            else {
                return;
            }
        } else {
            const long raw = static_cast<long>(atom_getfloat(argv));
            if (raw >= 1 && raw <= k_airfx_totape6_tilde_param_count) {
                index = raw - 1;
            } else {
                index = raw;
            }
        }
        value = atom_getfloat(argv + 1);
    }

    airfx_totape6_tilde_set_parameter_by_index(x, index, value);
}

static void airfx_totape6_tilde_params(t_airfx_totape6_tilde* x)
{
    object_post((t_object*)x, "airfx.totape6~ parameter map:");
    object_post((t_object*)x, "  1: input (Input) default=0.500000");
    object_post((t_object*)x, "  2: soften (Soften) default=0.500000");
    object_post((t_object*)x, "  3: head_b (Head B) default=0.500000");
    object_post((t_object*)x, "  4: flutter (Flutter) default=0.500000");
    object_post((t_object*)x, "  5: output (Output) default=0.500000");
    object_post((t_object*)x, "  6: dry_wet (Dry/Wet) default=1.000000");
}

static void airfx_totape6_tilde_assist(t_airfx_totape6_tilde* /*x*/, void* /*b*/, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        if (a == 0) {
            std::strncpy(s, "(signal/float) Left input, param 1 (Input)", k_assist_max_len);
        } else {
            std::strncpy(s, "(signal) Right input", k_assist_max_len);
        }
    } else {
        if (a == 0) {
            std::strncpy(s, "(signal) Left output", k_assist_max_len);
        } else {
            std::strncpy(s, "(signal) Right output", k_assist_max_len);
        }
    }
    s[k_assist_max_len - 1] = '\0';
}

static void airfx_totape6_tilde_perform64(
    t_airfx_totape6_tilde* x,
    t_object* /*dsp64*/,
    double** ins,
    long numins,
    double** outs,
    long numouts,
    long sampleframes,
    long /*flags*/,
    void* /*userparam*/)
{
    if (numins < 2 || numouts < 2) {
        return;
    }

    if (x->obj.z_disabled) {
        if (ins[0] && outs[0] && ins[0] != outs[0]) {
            std::memcpy(outs[0], ins[0], static_cast<size_t>(sampleframes) * sizeof(double));
        }
        if (ins[1] && outs[1] && ins[1] != outs[1]) {
            std::memcpy(outs[1], ins[1], static_cast<size_t>(sampleframes) * sizeof(double));
        }
        return;
    }

    x->engine.process(ins, outs, static_cast<int>(sampleframes));
}

static void airfx_totape6_tilde_dsp64(
    t_airfx_totape6_tilde* x,
    t_object* dsp64,
    short* /*count*/,
    double samplerate,
    long /*maxvectorsize*/,
    long /*flags*/)
{
    if (samplerate > 0.0 && samplerate != x->sample_rate) {
        x->sample_rate = samplerate;
        x->engine.reset(x->sample_rate);
        x->engine.setParameterByIndex(0, x->param_0);
        x->engine.setParameterByIndex(1, x->param_1);
        x->engine.setParameterByIndex(2, x->param_2);
        x->engine.setParameterByIndex(3, x->param_3);
        x->engine.setParameterByIndex(4, x->param_4);
        x->engine.setParameterByIndex(5, x->param_5);
    }

    object_method(dsp64, gensym("dsp_add64"), x, airfx_totape6_tilde_perform64, 0, nullptr);
}

extern "C" void ext_main(void* r)
{
    t_class* c = class_new(
        "airfx.totape6~",
        (method)airfx_totape6_tilde_new,
        (method)airfx_totape6_tilde_free,
        sizeof(t_airfx_totape6_tilde),
        0L,
        A_GIMME,
        0);

    class_addmethod(c, (method)airfx_totape6_tilde_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_int, "int", A_LONG, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_param, "param", A_GIMME, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_params, "params", 0);
    class_addmethod(c, (method)airfx_totape6_tilde_named_param_0, "input", A_FLOAT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_named_param_1, "soften", A_FLOAT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_named_param_2, "head_b", A_FLOAT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_named_param_3, "flutter", A_FLOAT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_named_param_4, "output", A_FLOAT, 0);
    class_addmethod(c, (method)airfx_totape6_tilde_named_param_5, "dry_wet", A_FLOAT, 0);

    CLASS_ATTR_DOUBLE(c, "input", 0, t_airfx_totape6_tilde, param_0);
    CLASS_ATTR_ACCESSORS(c, "input", nullptr, (method)airfx_totape6_tilde_attr_set_0);
    CLASS_ATTR_LABEL(c, "input", 0, "Input");
    CLASS_ATTR_FILTER_CLIP(c, "input", 0.0, 1.0);
    CLASS_ATTR_SAVE(c, "input", 1);
    CLASS_ATTR_DOUBLE(c, "soften", 0, t_airfx_totape6_tilde, param_1);
    CLASS_ATTR_ACCESSORS(c, "soften", nullptr, (method)airfx_totape6_tilde_attr_set_1);
    CLASS_ATTR_LABEL(c, "soften", 0, "Soften");
    CLASS_ATTR_FILTER_CLIP(c, "soften", 0.0, 1.0);
    CLASS_ATTR_SAVE(c, "soften", 1);
    CLASS_ATTR_DOUBLE(c, "head_b", 0, t_airfx_totape6_tilde, param_2);
    CLASS_ATTR_ACCESSORS(c, "head_b", nullptr, (method)airfx_totape6_tilde_attr_set_2);
    CLASS_ATTR_LABEL(c, "head_b", 0, "Head B");
    CLASS_ATTR_FILTER_CLIP(c, "head_b", 0.0, 1.0);
    CLASS_ATTR_SAVE(c, "head_b", 1);
    CLASS_ATTR_DOUBLE(c, "flutter", 0, t_airfx_totape6_tilde, param_3);
    CLASS_ATTR_ACCESSORS(c, "flutter", nullptr, (method)airfx_totape6_tilde_attr_set_3);
    CLASS_ATTR_LABEL(c, "flutter", 0, "Flutter");
    CLASS_ATTR_FILTER_CLIP(c, "flutter", 0.0, 1.0);
    CLASS_ATTR_SAVE(c, "flutter", 1);
    CLASS_ATTR_DOUBLE(c, "output", 0, t_airfx_totape6_tilde, param_4);
    CLASS_ATTR_ACCESSORS(c, "output", nullptr, (method)airfx_totape6_tilde_attr_set_4);
    CLASS_ATTR_LABEL(c, "output", 0, "Output");
    CLASS_ATTR_FILTER_CLIP(c, "output", 0.0, 1.0);
    CLASS_ATTR_SAVE(c, "output", 1);
    CLASS_ATTR_DOUBLE(c, "dry_wet", 0, t_airfx_totape6_tilde, param_5);
    CLASS_ATTR_ACCESSORS(c, "dry_wet", nullptr, (method)airfx_totape6_tilde_attr_set_5);
    CLASS_ATTR_LABEL(c, "dry_wet", 0, "Dry/Wet");
    CLASS_ATTR_FILTER_CLIP(c, "dry_wet", 0.0, 1.0);
    CLASS_ATTR_SAVE(c, "dry_wet", 1);

    class_dspinit(c);
    class_register(CLASS_BOX, c);
    s_airfx_totape6_tilde_class = c;

    object_post(nullptr, "airfx.totape6~: loaded (6 params)");
}
