/*
 * Copyright (C) 2021 Alyssa Rosenzweig <alyssa@rosenzweig.io>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __AGX_SELECTOR_H
#define __AGX_SELECTOR_H

#include <IOKit/IODataQueueClient.h>

enum agx_selector {
	AGX_SELECTOR_SET_API = 0x7,
	AGX_SELECTOR_CREATE_COMMAND_QUEUE = 0x8,
	AGX_SELECTOR_ALLOCATE_MEM = 0xA,
	AGX_SELECTOR_CREATE_CMDBUF = 0xF,
	AGX_SELECTOR_CREATE_NOTIFICATION_QUEUE = 0x11,
	AGX_SELECTOR_SUBMIT_COMMAND_BUFFERS = 0x1E,
	AGX_SELECTOR_GET_VERSION = 0x23,
	AGX_NUM_SELECTORS = 0x30
};

static const char *selector_table[AGX_NUM_SELECTORS] = {
	"unk0",
	"unk1",
	"unk2",
	"unk3",
	"unk4",
	"unk5",
	"unk6",
	"SET_API",
	"CREATE_COMMAND_QUEUE",
	"unk9",
	"ALLOCATE_MEM",
	"unkB",
	"unkC",
	"unkD",
	"unkE",
	"CREATE_CMDBUF",
	"unk10",
	"CREATE_NOTIFICATION_QUEUE",
	"unk12",
	"unk13",
	"unk14",
	"unk15",
	"unk16",
	"unk17",
	"unk18",
	"unk19",
	"unk1A",
	"unk1B",
	"unk1C",
	"unk1D",
	"SUBMIT_COMMAND_BUFFERS",
	"unk1F",
	"unk20",
	"unk21",
	"unk22",
	"GET_VERSION",
	"unk24",
	"unk25",
	"unk26",
	"unk27",
	"unk28",
	"unk29",
	"unk2A",
	"unk2B",
	"unk2C",
	"unk2D",
	"unk2E",
	"unk2F"
};

static inline const char *
wrap_selector_name(uint32_t selector)
{
	return (selector < AGX_NUM_SELECTORS) ? selector_table[selector] : "unk??";
}

struct agx_create_command_queue_resp {
	uint64_t id;
	uint32_t unk2; // 90 0A 08 27
	uint32_t unk3; // 0
} __attribute__((packed));

struct agx_create_cmdbuf_resp {
	void *map;
	uint32_t size;
	uint32_t id;
} __attribute__((packed));

struct agx_create_notification_queue_resp {
	IODataQueueMemory *queue;
	uint32_t unk2; // 1
	uint32_t unk3; // 0
} __attribute__((packed));

struct agx_submit_cmdbuf_req {
	uint32_t unk0;
	uint32_t unk1;
	uint32_t cmdbuf;
	uint32_t mappings;
	void *user_0;
	void *user_1;
	uint32_t unk2;
	uint32_t unk3;
} __attribute__((packed));

/* Memory allocation isn't really understood yet. By comparing SHADER/CMDBUF_32
 * vs everything else, it appears the 0x40000000 bit indicates the GPU VA must
 * be be in the first 4GiB */

enum agx_memory_type {
	AGX_MEMORY_TYPE_NORMAL      = 0x00000000, /* used for user allocations */
	AGX_MEMORY_TYPE_UNK         = 0x08000000, /* unknown */
	AGX_MEMORY_TYPE_CMDBUF_64   = 0x18000000, /* used for command buffer storage */
	AGX_MEMORY_TYPE_SHADER      = 0x48000000, /* used for shader memory, with VA = 0 */
	AGX_MEMORY_TYPE_CMDBUF_32   = 0x58000000, /* used for command buffers, with VA < 32-bit */
	AGX_MEMORY_TYPE_FRAMEBUFFER = 0x00888F00, /* used for framebuffer backing */
};

static inline const char *
agx_memory_type_name(uint32_t type)
{
	switch (type) {
	case AGX_MEMORY_TYPE_NORMAL: return "normal";
	case AGX_MEMORY_TYPE_UNK: return "unk";
	case AGX_MEMORY_TYPE_CMDBUF_64: return "cmdbuf_64";
	case AGX_MEMORY_TYPE_SHADER: return "shader";
	case AGX_MEMORY_TYPE_CMDBUF_32: return "cmdbuf_32";
	case AGX_MEMORY_TYPE_FRAMEBUFFER: return "framebuffer";
	default: return NULL;
	}
}

#endif
