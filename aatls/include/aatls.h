#ifndef AATLS_H
#define AATLS_H

#include <stdint.h>

/**
 * @file aatls.h
 * @brief Arc's texture atlas manipulation library
 *
 * libaatls provides parsing, serialization and in-memory manipulation
 * of texture atlas data structures.
 *
 * ---------------------------------------------------------------------------
 * Initialization
 * ---------------------------------------------------------------------------
 * All public structures MUST be zero-initialized before use.
 *
 * Example:
 * @code
 * aatls_atlas_t atlas = {0};
 * @endcode
 *
 * ---------------------------------------------------------------------------
 * Ownership Model
 * ---------------------------------------------------------------------------
 * - All `name` fields are heap-allocated and owned by their struct.
 * - Dynamic arrays (`regions`, `pages`) are owned by their parent struct.
 * - Insert functions use MOVE semantics (no deep copy).
 * - Copy functions perform deep copies.
 * - destroy_*() frees internal allocations but NOT the struct itself.
 *
 * ---------------------------------------------------------------------------
 * Thread Safety
 * ---------------------------------------------------------------------------
 * This library is NOT thread-safe.
 * External synchronization is required if accessed concurrently.
 */

/**
 * @enum aatls_err_t
 * @brief Error codes returned by libaatls functions.
 */
typedef enum {
    AATLS_ERR_OK = 0,        /**< Operation completed successfully */
    AATLS_ERR_INVALID_ARG,   /**< Invalid argument passed to function */
    AATLS_ERR_IO,            /**< I/O failure during file operations */
    AATLS_ERR_OUT_OF_MEM,    /**< Memory allocation failed */
    AATLS_ERR_FORMAT,        /**< Invalid or unsupported file format */
    AATLS_ERR_CORRUPT,       /**< Corrupted or inconsistent data */
    __AATLS_ERR_END__
} aatls_err_t;

/**
 * @enum aatls_wrap_t
 * @brief Texture wrapping modes.
 *
 * Maps to corresponding OpenGL wrap enums via aatls_gl_wrap().
 */
typedef enum {
    AATLS_WRAP_MIRRORED_REPEAT, /**< GL_MIRRORED_REPEAT */
    AATLS_WRAP_CLAMP_TO_EDGE,   /**< GL_CLAMP_TO_EDGE */
    AATLS_WRAP_REPEAT,          /**< GL_REPEAT */
    __AATLS_WRAP_END__
} aatls_wrap_t;

/**
 * @enum aatls_filter_t
 * @brief Texture filtering modes.
 *
 * Maps to corresponding OpenGL filter enums via aatls_gl_filter().
 */
typedef enum {
    AATLS_FILTER_NEAREST,                /**< GL_NEAREST */
    AATLS_FILTER_LINEAR,                 /**< GL_LINEAR */
    AATLS_FILTER_MIPMAP,                 /**< GL_LINEAR_MIPMAP_LINEAR */
    AATLS_FILTER_MIPMAP_NEAREST_NEAREST, /**< GL_NEAREST_MIPMAP_NEAREST */
    AATLS_FILTER_MIPMAP_LINEAR_NEAREST,  /**< GL_LINEAR_MIPMAP_NEAREST */
    AATLS_FILTER_MIPMAP_LINEAR_LINEAR,   /**< GL_LINEAR_MIPMAP_LINEAR */
    __AATLS_FILTER_END__
} aatls_filter_t;

/**
 * @struct aatls_region_t
 * @brief Describes a single region inside a texture page.
 *
 * Represents a rectangular sub-area of a texture page,
 * optionally including offset, split, and padding metadata.
 *
 * All optional fields are controlled by corresponding flags.
 *
 * The `name` field is heap-allocated and owned by this structure.
 */
typedef struct {
    int16_t left;    /**< X coordinate within page */
    int16_t top;     /**< Y coordinate within page */
    int16_t width;   /**< Region width */
    int16_t height;  /**< Region height */

    int16_t offset_x;        /**< X offset (if has_offsets) */
    int16_t offset_y;        /**< Y offset (if has_offsets) */
    int16_t original_width;  /**< Original untrimmed width */
    int16_t original_height; /**< Original untrimmed height */

    int16_t splits[4]; /**< 9-patch splits {left,right,top,bottom} */
    int16_t pads[4];   /**< 9-patch padding {left,right,top,bottom} */

    int8_t has_offsets; /**< Non-zero if offset fields are valid */
    int8_t has_splits;  /**< Non-zero if splits[] is valid */
    int8_t has_pads;    /**< Non-zero if pads[] is valid */

    int8_t flip;        /**< Non-zero if region is vertically flipped */

    char *name;         /**< Null-terminated region name (owned) */
} aatls_region_t;

/**
 * @struct aatls_page_t
 * @brief Represents a texture page containing multiple regions.
 *
 * Pages contain texture metadata and a dynamic array of regions.
 *
 * `regions` is a dynamically allocated array owned by this structure.
 */
typedef struct {
    char *name; /**< Texture page name (owned) */

    int16_t width;  /**< Texture width */
    int16_t height; /**< Texture height */

    aatls_filter_t filter_min; /**< Minification filter */
    aatls_filter_t filter_mag; /**< Magnification filter */

    aatls_wrap_t u_wrap; /**< Horizontal wrap mode */
    aatls_wrap_t v_wrap; /**< Vertical wrap mode */

    int32_t region_count;    /**< Number of regions */
    int32_t region_capacity; /**< Allocated capacity */

    aatls_region_t *regions; /**< Dynamic region array (owned) */
} aatls_page_t;

/**
 * @struct aatls_atlas_t
 * @brief Root atlas structure containing multiple pages.
 *
 * An atlas consists of one or more texture pages.
 */
typedef struct {
    int8_t version; /**< File format version */

    int32_t page_count;    /**< Number of pages */
    int32_t page_capacity; /**< Allocated capacity */

    aatls_page_t *pages; /**< Dynamic page array (owned) */
} aatls_atlas_t;

/**
 * @brief Convert wrap mode to OpenGL enum.
 * @param wrap Wrap mode.
 * @return Corresponding OpenGL constant.
 */
static inline unsigned int aatls_gl_wrap(
        const aatls_wrap_t wrap
)
{
    switch(wrap)
    {
        case AATLS_WRAP_MIRRORED_REPEAT: 
            return 0x8370; /* GL_MIRRORED_REPEAT */
        case AATLS_WRAP_CLAMP_TO_EDGE:
            return 0x812F; /* GL_CLAMP_TO_EDGE */
        case AATLS_WRAP_REPEAT:
            return 0x2901; /* GL_REPEAT */
        default:
            return 0x812F;
    }
}

/**
 * @brief Convert filter mode to OpenGL enum.
 * @param filter Filter mode.
 * @return Corresponding OpenGL constant.
 */
static inline unsigned int aatls_gl_filter(
        const aatls_filter_t filter
)
{
    switch(filter)
    {
        case AATLS_FILTER_NEAREST:
            return 0x2600; /* GL_NEAREST */
        case AATLS_FILTER_LINEAR:
            return 0x2601; /* GL_LINEAR */
        case AATLS_FILTER_MIPMAP:
        case AATLS_FILTER_MIPMAP_LINEAR_LINEAR:
            return 0x2703; /* GL_LINEAR_MIPMAP_LINEAR */
        case AATLS_FILTER_MIPMAP_NEAREST_NEAREST:
            return 0x2700; /* GL_NEAREST_MIPMAP_NEAREST */
        case AATLS_FILTER_MIPMAP_LINEAR_NEAREST:
            return 0x2701; /* GL_LINEAR_MIPMAP_NEAREST */
        default:
            return 0x2601;
    }
}

/**
 * @brief Convert error code to human-readable string.
 */
const char* aatls_strerror(
        const aatls_err_t err
);

/**
 * @brief Parse atlas file from disk.
 *
 * Reads and parses an atlas file from the filesystem.
 *
 * @param file_name Path to input file (null-terminated string).
 * @param[out] out Output atlas. Must be non-NULL and zero-initialized.
 *
 * On success, @p out becomes fully initialized and owns all allocations.
 * On failure, @p out remains in a valid zeroed state.
 *
 * @return
 *   - AATLS_ERR_OK on success
 *   - AATLS_ERR_INVALID_ARG if parameters are invalid
 *   - AATLS_ERR_IO on file read failure
 *   - AATLS_ERR_FORMAT if file format is invalid
 *   - AATLS_ERR_CORRUPT if data is inconsistent
 *   - AATLS_ERR_OUT_OF_MEM if allocation fails
 */
aatls_err_t aatls_parse(
        const char *file_name, 
        aatls_atlas_t *out
);

/**
 * @brief Parse atlas from memory buffer.
 *
 * @param buf Input buffer.
 * @param bufsz Size of input buffer in bytes.
 * @param[out] out Output atlas. Must be non-NULL and zero-initialized.
 *
 * The buffer is read-only and not retained after parsing.
 *
 * On success, @p out owns all allocated resources.
 *
 * @return Same error codes as aatls_parse().
 */
aatls_err_t aatls_parse_from_memory(
        const uint8_t *buf, 
        const uint32_t bufsz,
        aatls_atlas_t *out
);

/**
 * @brief Write atlas to disk.
 *
 * Serializes an atlas and writes it to a file.
 *
 * @param file_name Output file path.
 * @param atlas Input atlas. Must be non-NULL and valid.
 *
 * @return
 *   - AATLS_ERR_OK on success
 *   - AATLS_ERR_INVALID_ARG if parameters are invalid
 *   - AATLS_ERR_IO on write failure
 *   - AATLS_ERR_OUT_OF_MEM if temporary allocation fails
 */
aatls_err_t aatls_write(
        const char *file_name,
        const aatls_atlas_t *atlas
);

/**
 * @brief Serialize atlas into memory.
 *
 * Allocates a buffer containing the serialized atlas.
 *
 * @param atlas Input atlas.
 * @param[out] out_buf Receives allocated buffer (caller owns).
 * @param[out] out_size Receives buffer size in bytes.
 *
 * Caller must free(*out_buf) when no longer needed.
 *
 * @return
 *   - AATLS_ERR_OK on success
 *   - AATLS_ERR_INVALID_ARG if parameters are invalid
 *   - AATLS_ERR_OUT_OF_MEM if allocation fails
 */
aatls_err_t aatls_write_to_memory(
        const aatls_atlas_t *atlas,
        uint8_t **out_buf,
        uint32_t *out_size
);

/**
 * @brief Deep copy a region.
 *
 * All internal resources (including name) are duplicated.
 *
 * @param src Source region (non-NULL).
 * @param[out] dst Destination region (must be zero-initialized).
 *
 * On success, @p dst becomes a fully independent copy.
 * On failure, @p dst remains unchanged.
 *
 * @return
 *   - AATLS_ERR_OK
 *   - AATLS_ERR_INVALID_ARG
 *   - AATLS_ERR_OUT_OF_MEM
 */
aatls_err_t aatls_copy_region(
        const aatls_region_t *src, 
        aatls_region_t *dst
);

/**
 * @brief Deep copy a page.
 *
 * All internal resources (including name) are duplicated.
 *
 * @param src Source page (non-NULL).
 * @param[out] dst Destination page (must be zero-initialized).
 *
 * On success, @p dst becomes a fully independent copy.
 * On failure, @p dst remains unchanged.
 *
 * @return
 *   - AATLS_ERR_OK
 *   - AATLS_ERR_INVALID_ARG
 *   - AATLS_ERR_OUT_OF_MEM
 */
aatls_err_t aatls_copy_page(
        const aatls_page_t *src,
        aatls_page_t *dst
);

/**
 * @brief Deep copy a atlas.
 *
 * All internal resources (including name) are duplicated.
 *
 * @param src Source atlas (non-NULL).
 * @param[out] dst Destination atlas (must be zero-initialized).
 *
 * On success, @p dst becomes a fully independent copy.
 * On failure, @p dst remains unchanged.
 *
 * @return
 *   - AATLS_ERR_OK
 *   - AATLS_ERR_INVALID_ARG
 *   - AATLS_ERR_OUT_OF_MEM
 */
aatls_err_t aatls_copy_atlas(
        const aatls_atlas_t *src,
        aatls_atlas_t *dst
);

/**
 * @brief Move a region into a page at the specified index.
 *
 * Transfers ownership of all internal resources from @p region
 * into @p page. No deep copy is performed.
 *
 * Index Behavior:
 *   - idx < 0 → treated as 0
 *   - idx > region_count → treated as region_count (append)
 *
 * After clamping, elements at and after the insertion position
 * are shifted one position toward higher indices.
 *
 * On Success:
 *   - Ownership of @p region is transferred to @p page.
 *   - @p region is reset to zero (valid but empty state).
 *   - Caller MUST NOT access or destroy internal data previously owned by @p region.
 *
 * On Failure:
 *   - @p region remains unchanged.
 *   - No ownership transfer occurs.
 *
 * Complexity: O(n) due to element shifting.
 *
 * @param page Target page (non-NULL).
 * @param region Region to move (non-NULL).
 * @param idx Desired insertion index (clamped).
 *
 * @return
 *   - AATLS_ERR_OK
 *   - AATLS_ERR_INVALID_ARG
 *   - AATLS_ERR_OUT_OF_MEM
 */
aatls_err_t aatls_insert_region(
        aatls_page_t *page,
        aatls_region_t *region,
        const int32_t idx
);

/**
 * @brief Move a page into an atlas at the specified index.
 *
 * Transfers ownership of all internal resources from @p page
 * into @p atlas. No deep copy is performed.
 *
 * Index Behavior:
 *   - idx < 0 → treated as 0
 *   - idx > page_count → treated as page_count (append)
 *
 * After clamping, elements at and after the insertion position
 * are shifted one position toward higher indices.
 *
 * On Success:
 *   - Ownership of @p page is transferred to @p atlas.
 *   - @p page is reset to zero.
 *   - Caller MUST NOT access or destroy internal data previously owned by @p page.
 *
 * On Failure:
 *   - @p page remains unchanged.
 *
 * Complexity: O(n)
 *
 * @param atlas Target atlas (non-NULL).
 * @param page Page to move (non-NULL).
 * @param idx Desired insertion index (clamped).
 *
 * @return
 *   - AATLS_ERR_OK
 *   - AATLS_ERR_INVALID_ARG
 *   - AATLS_ERR_OUT_OF_MEM
 */
aatls_err_t aatls_insert_page(
        aatls_atlas_t *atlas,
        aatls_page_t *page,
        const int32_t idx
);

/**
 * @brief Find region index inside a page by name.
 *
 * Performs a linear search.
 *
 * @param page Page to search.
 * @param name Null-terminated region name.
 *
 * @return
 *   - Region index (>= 0) if found
 *   - -1 if not found
 */
int32_t aatls_find_region_in_page(
        const aatls_page_t *page,
        const char *name
);

/**
 * @brief Find region by name across all pages.
 *
 * Performs a linear search across pages and regions.
 *
 * @param atlas Atlas to search.
 * @param name Region name.
 * @param[out] region_idx Receives region index within the page (if found).
 *
 * @return
 *   - Page index (>= 0) if found
 *   - -1 if not found
 */
int32_t aatls_find_region_in_atlas(
        const aatls_atlas_t *atlas,
        const char *name,
        int32_t *region_idx
);

/**
 * @brief Find page index by name.
 *
 * Performs a linear search.
 *
 * @param atlas Atlas to search.
 * @param name Page name.
 *
 * @return
 *   - Page index (>= 0) if found
 *   - -1 if not found
 */
int32_t aatls_find_page_in_atlas(
        const aatls_atlas_t *atlas,
        const char *name
);

/**
 * @brief Destroy a region.
 *
 * Frees all internal allocations owned by the region,
 * including its name.
 *
 * The struct itself is not freed.
 *
 * Safe to call on a zero-initialized region.
 */
void aatls_destroy_region(
        aatls_region_t *region
);

/**
 * @brief Destroy a page.
 *
 * Frees:
 *   - All contained regions
 *   - Region array
 *   - Page name
 *
 * The struct itself is not freed.
 *
 * Safe to call on a zero-initialized page.
 */
void aatls_destroy_page(
        aatls_page_t *page
);

/**
 * @brief Destroy an atlas.
 *
 * Frees:
 *   - All contained pages
 *   - Page array
 *
 * The struct itself is not freed.
 *
 * Safe to call on a zero-initialized atlas.
 */
void aatls_destroy_atlas(
        aatls_atlas_t *atlas
);

#endif
