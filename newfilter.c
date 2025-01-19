#include <gst/gst.h>
#include <stdio.h>
#include <string.h>

typedef struct _DuplicateFilter {
  GstElement element;
  GstPad *sinkpad, *srcpad1, *srcpad2;
  GstElement *queue1, *audioconvert1, *audio_sink;
  GstElement *queue2, *audioconvert2, *wavenc, *file_sink;
  GstCaps *caps;
} DuplicateFilter;

typedef struct _DuplicateFilterClass {
  GstElementClass parent_class;
} DuplicateFilterClass;

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw"));

static GstStaticPadTemplate src_template1 = GST_STATIC_PAD_TEMPLATE("src_0",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw"));

static GstStaticPadTemplate src_template2 = GST_STATIC_PAD_TEMPLATE("src_1",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS("audio/x-raw"));

G_DEFINE_TYPE(DuplicateFilter, duplicate_filter, GST_TYPE_ELEMENT);

static GstFlowReturn duplicate_filter_chain(GstPad *pad, GstObject *parent, GstBuffer *buf);
static gboolean duplicate_filter_event(GstPad *pad, GstObject *parent, GstEvent *event);

static void duplicate_filter_class_init(DuplicateFilterClass *klass) {
  GstElementClass *element_class = GST_ELEMENT_CLASS(klass);

  gst_element_class_set_details_simple(element_class,
      "Duplicate Filter",
      "Filter/Effect",
      "Duplicates incoming buffers to two source pads",
      "Author <author@example.com>");

  gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&sink_template));
  gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&src_template1));
  gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&src_template2));
}

static void duplicate_filter_init(DuplicateFilter *filter) {
  filter->sinkpad = gst_pad_new_from_static_template(&sink_template, "sink");
  gst_pad_set_chain_function(filter->sinkpad, GST_DEBUG_FUNCPTR(duplicate_filter_chain));
  gst_pad_set_event_function(filter->sinkpad, GST_DEBUG_FUNCPTR(duplicate_filter_event));
  gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);

  filter->srcpad1 = gst_pad_new_from_static_template(&src_template1, "src_0");
  gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad1);

  filter->srcpad2 = gst_pad_new_from_static_template(&src_template2, "src_1");
  gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad2);

  filter->queue1 = gst_element_factory_make("queue", "queue1");
  filter->audioconvert1 = gst_element_factory_make("audioconvert", "audioconvert1");
  filter->audio_sink = gst_element_factory_make("autoaudiosink", "audio_sink");

  filter->queue2 = gst_element_factory_make("queue", "queue2");
  filter->audioconvert2 = gst_element_factory_make("audioconvert", "audioconvert2");
  filter->wavenc = gst_element_factory_make("wavenc", "wavenc");
  filter->file_sink = gst_element_factory_make("filesink", "file_sink");

  gst_bin_add_many(GST_BIN(filter), filter->queue1, filter->audioconvert1, filter->audio_sink, filter->queue2, filter->audioconvert2, filter->wavenc, filter->file_sink, NULL);
  gst_element_link_many(filter->queue1, filter->audioconvert1, filter->audio_sink, NULL);
  gst_element_link_many(filter->queue2, filter->audioconvert2, filter->wavenc, filter->file_sink, NULL);

  g_object_set(filter->file_sink, "location", "output.wav", NULL);
}

static gboolean duplicate_filter_event(GstPad *pad, GstObject *parent, GstEvent *event) {
  DuplicateFilter *filter = (DuplicateFilter *)parent;

  switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_CAPS: {
      GstCaps *caps;
      gst_event_parse_caps(event, &caps);
      if (filter->caps) {
        gst_caps_unref(filter->caps);
      }
      filter->caps = gst_caps_ref(caps);
      gst_pad_set_caps(filter->srcpad1, caps);
      gst_pad_set_caps(filter->srcpad2, caps);
      break;
    }
    default:
      break;
  }

  return gst_pad_event_default(pad, parent, event);
}

static GstFlowReturn duplicate_filter_chain(GstPad *pad, GstObject *parent, GstBuffer *buf) {
  DuplicateFilter *filter = (DuplicateFilter *)parent;

  g_print("duplicate_filter_chain: Received buffer of size %zu\n", gst_buffer_get_size(buf));

  if (!filter->caps) {
    g_printerr("No caps set on sink pad\n");
    return GST_FLOW_ERROR;
  }

  /* Push buffer to first src pad */
  GstBuffer *copy1 = gst_buffer_copy(buf);
  if (!copy1) {
    g_printerr("Failed to copy buffer for src_0\n");
    return GST_FLOW_ERROR;
  }
  GstFlowReturn ret1 = gst_pad_push(filter->srcpad1, copy1);
  if (ret1 != GST_FLOW_OK) {
    g_printerr("Failed to push buffer to src_0\n");
    gst_buffer_unref(copy1);
    return ret1;
  }

  /* Push buffer to second src pad */
  GstBuffer *copy2 = gst_buffer_copy(buf);
  if (!copy2) {
    g_printerr("Failed to copy buffer for src_1\n");
    return GST_FLOW_ERROR;
  }
  GstFlowReturn ret2 = gst_pad_push(filter->srcpad2, copy2);
  if (ret2 != GST_FLOW_OK) {
    g_printerr("Failed to push buffer to src_1\n");
    gst_buffer_unref(copy2);
    return ret2;
  }

  gst_buffer_unref(buf);
  return GST_FLOW_OK;
}

static gboolean duplicate_filter_plugin_init(GstPlugin *plugin) {
  return gst_element_register(plugin, "duplicatefilter", GST_RANK_NONE, duplicate_filter_get_type());
}

#define PACKAGE "duplicatefilter"
GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    duplicatefilter,
    "Duplicates incoming buffers to two outputs",
    duplicate_filter_plugin_init,
    "1.0",
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
);