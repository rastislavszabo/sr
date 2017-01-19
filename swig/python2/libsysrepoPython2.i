%module libsysrepoPython2

%include <stdint.i>

/* Filter out 'Setting a const char * variable may leak memory' warnings */
%warnfilter(451);

/* Filter out 'Identifier '~Subscribe' redefined by %extend (ignored)'*/
%warnfilter(302);

%{
    extern "C" {
        #include "../inc/sysrepo.h"
    }

%}

%include <std_except.i>
%catches(std::runtime_error, std::exception, std::string);

%inline %{
#include <unistd.h>
#include "../inc/sysrepo.h"
#include <signal.h>
#include <vector>
#include <memory>

#include "Sysrepo.h"
#include "Struct.h"
#include "Session.h"


/* custom infinite loop */
volatile int exit_application = 0;

static void
sigint_handler(int signum)
{
    exit_application = 1;
}


static void global_loop() {
    /* loop until ctrl-c is pressed / SIGINT is received */
    signal(SIGINT, sigint_handler);
    while (!exit_application) {
        sleep(1000);  /* or do some more useful work... */
    }
}

class Wrap_cb {
public:
    Wrap_cb(PyObject *callback): _callback(NULL) {

        if (!PyCallable_Check(callback)) {
            throw std::runtime_error("Python Object is not callable.\n");
        }
        else {
            _callback = callback;
            Py_XINCREF(_callback);
        }
    }
    ~Wrap_cb() {
        if(_callback)
            Py_XDECREF(_callback);
    }

    int module_change_subscribe(sr_session_ctx_t *session, const char *module_name, sr_notif_event_t event, \
                                 void *private_ctx) {
        PyObject *arglist;

        Session *sess = (Session *)new Session(session);
        S_Session *shared_sess = sess ? new S_Session(sess) : 0;
        PyObject *s = SWIG_NewPointerObj(SWIG_as_voidptr(shared_sess), SWIGTYPE_p_std__shared_ptrT_Session_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(OsiO)", s, module_name, event, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            sess->~Session();
            throw std::runtime_error("Python callback module_change_subscribe failed.\n");
        } else {
            sess->~Session();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
    }

    int subtree_change(sr_session_ctx_t *session, const char *xpath, sr_notif_event_t event,\
                       void *private_ctx) {
        PyObject *arglist;

        Session *sess = (Session *)new Session(session);
        S_Session *shared_sess = sess ? new S_Session(sess) : 0;
        PyObject *s = SWIG_NewPointerObj(SWIG_as_voidptr(shared_sess), SWIGTYPE_p_std__shared_ptrT_Session_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(OsiO)", s, xpath, event, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            sess->~Session();
            throw std::runtime_error("Python callback subtree_change failed.\n");
        } else {
            sess->~Session();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
    }

    void module_install(const char *module_name, const char *revision, sr_module_state_t state, void *private_ctx) {
        PyObject *arglist;
        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(ssOO)", module_name, revision, state, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL)
            throw std::runtime_error("Python callback module_install failed.\n");
        else
            Py_DECREF(result);
    }

    void feature_enable(const char *module_name, const char *feature_name, bool enabled, void *private_ctx) {
        PyObject *arglist;
        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(ssOO)", module_name, feature_name, enabled ? Py_True: Py_False, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL)
            throw std::runtime_error("Python feature_enable failed.\n");
        else
            Py_DECREF(result);
    }

    int rpc_cb(const char *xpath, const sr_val_t *input, const size_t input_cnt, sr_val_t **output,\
               size_t *output_cnt, void *private_ctx) {
        PyObject *arglist;

        Vals *in_vals =(Vals *)new Vals(input, input_cnt, NULL);
        Vals_Holder *out_vals =(Vals_Holder *)new Vals_Holder(output, output_cnt);

        shared_ptr<Vals> *shared_in_vals = in_vals ? new shared_ptr<Vals>(in_vals) : 0;
        PyObject *in = SWIG_NewPointerObj(SWIG_as_voidptr(shared_in_vals), SWIGTYPE_p_std__shared_ptrT_Vals_t, SWIG_POINTER_DISOWN);

        shared_ptr<Vals_Holder> *shared_out_vals = out_vals ? new shared_ptr<Vals_Holder>(out_vals) : 0;
        PyObject *out = SWIG_NewPointerObj(SWIG_as_voidptr(shared_out_vals), SWIGTYPE_p_std__shared_ptrT_Vals_Holder_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOOO)", xpath, in, out, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            in_vals->~Vals();
            out_vals->~Vals_Holder();
            throw std::runtime_error("Python callback rpc_cb failed.\n");
        } else {
            in_vals->~Vals();
            out_vals->~Vals_Holder();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
     }

    int action_cb(const char *xpath, const sr_val_t *input, const size_t input_cnt, sr_val_t **output,\
               size_t *output_cnt, void *private_ctx) {
        PyObject *arglist;

        Vals *in_vals =(Vals *)new Vals(input, input_cnt, NULL);
        Vals_Holder *out_vals =(Vals_Holder *)new Vals_Holder(output, output_cnt);

        shared_ptr<Vals> *shared_in_vals = in_vals ? new shared_ptr<Vals>(in_vals) : 0;
        PyObject *in = SWIG_NewPointerObj(SWIG_as_voidptr(shared_in_vals), SWIGTYPE_p_std__shared_ptrT_Vals_t, SWIG_POINTER_DISOWN);

        shared_ptr<Vals_Holder> *shared_out_vals = out_vals ? new shared_ptr<Vals_Holder>(out_vals) : 0;
        PyObject *out = SWIG_NewPointerObj(SWIG_as_voidptr(shared_out_vals), SWIGTYPE_p_std__shared_ptrT_Vals_Holder_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOOO)", xpath, in, out, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            in_vals->~Vals();
            out_vals->~Vals_Holder();
            throw std::runtime_error("Python callback action_cb failed.\n");
        } else {
            in_vals->~Vals();
            out_vals->~Vals_Holder();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
     }

    int rpc_tree_cb(const char *xpath, const sr_node_t *input, const size_t input_cnt,\
                         sr_node_t **output, size_t *output_cnt, void *private_ctx) {
        PyObject *arglist;

        Trees *in_vals =(Trees *)new Trees(input, input_cnt, NULL);
        Trees_Holder *out_vals =(Trees_Holder *)new Trees_Holder(output, output_cnt);
        shared_ptr<Trees> *shared_in_vals = in_vals ? new shared_ptr<Trees>(in_vals) : 0;
        PyObject *in = SWIG_NewPointerObj(SWIG_as_voidptr(shared_in_vals), SWIGTYPE_p_std__shared_ptrT_Trees_t, SWIG_POINTER_DISOWN);

        shared_ptr<Trees_Holder> *shared_out_vals = out_vals ? new shared_ptr<Trees_Holder>(out_vals) : 0;
        PyObject *out = SWIG_NewPointerObj(SWIG_as_voidptr(shared_out_vals), SWIGTYPE_p_std__shared_ptrT_Trees_Holder_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOOO)", xpath, in, out, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            in_vals->~Trees();
            out_vals->~Trees_Holder();
            throw std::runtime_error("Python callback rpc_tree_cb failed.\n");
        } else {
            in_vals->~Trees();
            out_vals->~Trees_Holder();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
    }

    int action_tree_cb(const char *xpath, const sr_node_t *input, const size_t input_cnt,\
                         sr_node_t **output, size_t *output_cnt, void *private_ctx) {
        PyObject *arglist;

        Trees *in_vals =(Trees *)new Trees(input, input_cnt, NULL);
        Trees_Holder *out_vals =(Trees_Holder *)new Trees_Holder(output, output_cnt);
        shared_ptr<Trees> *shared_in_vals = in_vals ? new shared_ptr<Trees>(in_vals) : 0;
        PyObject *in = SWIG_NewPointerObj(SWIG_as_voidptr(shared_in_vals), SWIGTYPE_p_std__shared_ptrT_Trees_t, SWIG_POINTER_DISOWN);

        shared_ptr<Trees_Holder> *shared_out_vals = out_vals ? new shared_ptr<Trees_Holder>(out_vals) : 0;
        PyObject *out = SWIG_NewPointerObj(SWIG_as_voidptr(shared_out_vals), SWIGTYPE_p_std__shared_ptrT_Trees_Holder_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOOO)", xpath, in, out, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            in_vals->~Trees();
            out_vals->~Trees_Holder();
            throw std::runtime_error("Python callback action_tree_cb failed.\n");
        } else {
            in_vals->~Trees();
            out_vals->~Trees_Holder();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
    }


    int dp_get_items(const char *xpath, sr_val_t **values, size_t *values_cnt, void *private_ctx) {
        PyObject *arglist;

        Vals_Holder *out_vals =(Vals_Holder *)new Vals_Holder(values, values_cnt);
        shared_ptr<Vals_Holder> *shared_out_vals = out_vals ? new shared_ptr<Vals_Holder>(out_vals) : 0;
        PyObject *out = SWIG_NewPointerObj(SWIG_as_voidptr(shared_out_vals), SWIGTYPE_p_std__shared_ptrT_Vals_Holder_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOO)", xpath, out, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            out_vals->~Vals_Holder();
            throw std::runtime_error("Python callback dp_get_items failed.\n");
        } else {
            out_vals->~Vals_Holder();
            int ret = SR_ERR_OK;
            if (result && PyInt_Check(result)) {
                ret = PyInt_AsLong(result);
            }
            Py_DECREF(result);
            return ret;
        }
    }

    void event_notif(const sr_ev_notif_type_t notif_type, const char *xpath, const sr_val_t *values, const size_t values_cnt, time_t timestamp, void *private_ctx) {
        PyObject *arglist;

        Vals *in_vals =(Vals *)new Vals(values, values_cnt, NULL);
        shared_ptr<Vals> *shared_in_vals = in_vals ? new shared_ptr<Vals>(in_vals) : 0;
        PyObject *in = SWIG_NewPointerObj(SWIG_as_voidptr(shared_in_vals), SWIGTYPE_p_std__shared_ptrT_Vals_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOO)", xpath, in, timestamp, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            in_vals->~Vals();
            throw std::runtime_error("Python callback event_notif failed.\n");
        } else {
            in_vals->~Vals();
            Py_DECREF(result);
        }
    }

    void event_notif_tree(const sr_ev_notif_type_t, const char *xpath, const sr_node_t *trees, const size_t tree_cnt, time_t timestamp, void *private_ctx) {
        PyObject *arglist;

        Trees *in_vals =(Trees *)new Trees(trees, tree_cnt, NULL);
        shared_ptr<Trees> *shared_in_vals = in_vals ? new shared_ptr<Trees>(in_vals) : 0;
        PyObject *in = SWIG_NewPointerObj(SWIG_as_voidptr(shared_in_vals), SWIGTYPE_p_std__shared_ptrT_Trees_t, SWIG_POINTER_DISOWN);

        PyObject *p =  SWIG_NewPointerObj(private_ctx, SWIGTYPE_p_void, 0);
        arglist = Py_BuildValue("(sOO)", xpath, in, timestamp, p);
        PyObject *result = PyEval_CallObject(_callback, arglist);
        Py_DECREF(arglist);
        if (result == NULL) {
            in_vals->~Trees();
            throw std::runtime_error("Python callback event_notif_tree failed.\n");
        } else {
            in_vals->~Trees();
            Py_DECREF(result);
        }
    }

    void *private_ctx;

private:
    PyObject *_callback;
};

static int g_module_change_subscribe_cb(sr_session_ctx_t *session, const char *module_name,\
                                        sr_notif_event_t event, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->module_change_subscribe(session, module_name, event, ctx->private_ctx);
}

static int g_subtree_change_cb(sr_session_ctx_t *session, const char *xpath, sr_notif_event_t event,\
                               void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->subtree_change(session, xpath, event, ctx->private_ctx);
}

static void g_module_install_cb(const char *module_name, const char *revision, sr_module_state_t state, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    ctx->module_install(module_name, revision, state, ctx->private_ctx);
}

static void g_feature_enable_cb(const char *module_name, const char *feature_name, bool enabled, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    ctx->feature_enable(module_name, feature_name, enabled, ctx->private_ctx);
}

static int g_rpc_cb(const char *xpath, const sr_val_t *input, const size_t input_cnt, sr_val_t **output,\
                     size_t *output_cnt, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->rpc_cb(xpath, input, input_cnt, output, output_cnt, ctx->private_ctx);
}

static int g_action_cb(const char *xpath, const sr_val_t *input, const size_t input_cnt, sr_val_t **output,\
                     size_t *output_cnt, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->action_cb(xpath, input, input_cnt, output, output_cnt, ctx->private_ctx);
}

static int g_rpc_tree_cb(const char *xpath, const sr_node_t *input, const size_t input_cnt,\
                         sr_node_t **output, size_t *output_cnt, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->rpc_tree_cb(xpath, input, input_cnt, output, output_cnt, ctx->private_ctx);
}

static int g_action_tree_cb(const char *xpath, const sr_node_t *input, const size_t input_cnt,\
                         sr_node_t **output, size_t *output_cnt, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->action_tree_cb(xpath, input, input_cnt, output, output_cnt, ctx->private_ctx);
}

static int g_dp_get_items_cb(const char *xpath, sr_val_t **values, size_t *values_cnt, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    return ctx->dp_get_items(xpath, values, values_cnt, ctx->private_ctx);
}

static void g_event_notif_cb(const sr_ev_notif_type_t notif_type, const char *xpath, const sr_val_t *values, const size_t values_cnt, time_t timestamp, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    ctx->event_notif(notif_type, xpath, values, values_cnt, timestamp, ctx->private_ctx);
}

static void g_event_notif_tree_cb(const sr_ev_notif_type_t notif_type, const char *xpath, const sr_node_t *trees, const size_t tree_cnt, time_t timestamp, void *private_ctx)
{
    Wrap_cb *ctx = (Wrap_cb *) private_ctx;
    ctx->event_notif_tree(notif_type, xpath, trees, tree_cnt, timestamp, ctx->private_ctx);
}


%}

%extend Subscribe {

    void module_change_subscribe(const char *module_name, PyObject *callback, void *private_ctx = NULL, \
                                 uint32_t priority = 0, sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        /* create class */
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_module_change_subscribe(self->swig_sess(), module_name, g_module_change_subscribe_cb, \
                                             class_ctx, priority, opts, self->swig_sub());
        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    };

    void subtree_change_subscribe(const char *xpath, PyObject *callback, void *private_ctx = NULL,\
                                 uint32_t priority = 0, sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        /* create class */
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_subtree_change_subscribe(self->swig_sess(), xpath, g_subtree_change_cb, class_ctx,\
                                              priority, opts, self->swig_sub());
        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void module_install_subscribe(PyObject *callback, void *private_ctx = NULL,\
                                  sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        /* create class */
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret =  sr_module_install_subscribe(self->swig_sess(), g_module_install_cb, class_ctx,
                                               opts, self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void feature_enable_subscribe(PyObject *callback, void *private_ctx = NULL,\
                                  sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        /* create class */
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_feature_enable_subscribe(self->swig_sess(), g_feature_enable_cb, class_ctx,
                                              opts, self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void rpc_subscribe(const char *xpath, PyObject *callback, void *private_ctx = NULL,\
                       sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_rpc_subscribe(self->swig_sess(), xpath, g_rpc_cb, class_ctx, opts,\
                                   self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void action_subscribe(const char *xpath, PyObject *callback, void *private_ctx = NULL,\
                       sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_action_subscribe(self->swig_sess(), xpath, g_action_cb, class_ctx, opts,\
                                   self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void rpc_subscribe_tree(const char *xpath, PyObject *callback, void *private_ctx = NULL,\
                       sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_rpc_subscribe_tree(self->swig_sess(), xpath, g_rpc_tree_cb, class_ctx, opts,\
                                   self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void action_subscribe_tree(const char *xpath, PyObject *callback, void *private_ctx = NULL,\
                       sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_action_subscribe_tree(self->swig_sess(), xpath, g_action_tree_cb, class_ctx, opts,\
                                   self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void event_notif_subscribe(const char *xpath, PyObject *callback, void *private_ctx,\
                               sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_event_notif_subscribe(self->swig_sess(), xpath, g_event_notif_cb, class_ctx, opts,\
                                   self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void event_notif_subscribe_tree(const char *xpath, PyObject *callback, void *private_ctx,\
                               sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_event_notif_subscribe_tree(self->swig_sess(), xpath, g_event_notif_tree_cb,\
                                                class_ctx, opts, self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void dp_get_items_subscribe(const char *xpath, PyObject *callback, void *private_ctx, \
                               sr_subscr_options_t opts = SUBSCR_DEFAULT) {
        Wrap_cb *class_ctx = NULL;
        class_ctx = new Wrap_cb(callback);

        self->wrap_cb_l.push_back(class_ctx);
        class_ctx->private_ctx = private_ctx;

        int ret = sr_dp_get_items_subscribe(self->swig_sess(), xpath, g_dp_get_items_cb, class_ctx,\
                                            opts, self->swig_sub());

        if (SR_ERR_OK != ret) {
            throw std::runtime_error(sr_strerror(ret));
        }
    }

    void additional_cleanup(void *private_ctx) {
        delete static_cast<Wrap_cb*>(private_ctx);
    }
};

%include "../swig_base/python_base.i"
