#include <gst/gst.h>

static void on_pad_added(GstElement *element, GstPad *new_pad, gpointer data) {
    GstElement *sink = (GstElement *)data;
    GstPad *sink_pad = gst_element_get_static_pad(sink, "sink");

    g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(element));

    if (!gst_pad_is_linked(sink_pad)) {
        GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
        if (ret != GST_PAD_LINK_OK) {
            g_printerr("Failed to link pads: %s\n", gst_pad_link_get_name(ret));
        } else {
            g_print("Successfully linked pad '%s' to '%s'.\n", GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(sink));
        }
    } else {
        g_print("Pad '%s' is already linked. Ignoring.\n", GST_PAD_NAME(new_pad));
    }
    gst_object_unref(sink_pad);
}

int main(int argc, char *argv[]) {
    GstElement *pipeline, *src, *decodebin, *duplicatefilter;
    GstBus *bus;
    GstMessage *msg;
    GMainLoop *loop;

    gst_init(&argc, &argv);

    pipeline = gst_pipeline_new("pipeline");

    src = gst_element_factory_make("filesrc", "src");
    decodebin = gst_element_factory_make("decodebin", "decodebin");
    duplicatefilter = gst_element_factory_make("duplicatefilter", "duplicatefilter");

    if (!pipeline || !src || !decodebin || !duplicatefilter) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    g_object_set(src, "location", "/home/vboxuser/PinkPanther30.wav", NULL);

    gst_bin_add_many(GST_BIN(pipeline), src, decodebin, duplicatefilter, NULL);
    gst_element_link(src, decodebin);

    g_signal_connect(decodebin, "pad-added", G_CALLBACK(on_pad_added), duplicatefilter);

    loop = g_main_loop_new(NULL, FALSE);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(pipeline);
    do {
        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_STATE_CHANGED);

        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE(msg)) {
                case GST_MESSAGE_ERROR:
                    gst_message_parse_error(msg, &err, &debug_info);
                    g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                    g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                    g_clear_error(&err);
                    g_free(debug_info);
                    break;
                case GST_MESSAGE_EOS:
                    g_print("End-Of-Stream reached.\n");
                    break;
                case GST_MESSAGE_STATE_CHANGED:
                    if (GST_MESSAGE_SRC(msg) == GST_OBJECT(pipeline)) {
                        GstState old_state, new_state, pending_state;
                        gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
                        g_print("Pipeline state changed from %s to %s:\n",
                                gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
                    }
                    break;
                default:
                    g_printerr("Unexpected message received.\n");
                    break;
            }
            gst_message_unref(msg);
        }
    } while (!msg || GST_MESSAGE_TYPE(msg) != GST_MESSAGE_EOS);

    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}