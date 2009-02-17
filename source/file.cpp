#include "stdafx.h"

#include "global_file.h"
#include "http_server/connection.hpp"
#include "file.h"

using namespace boost::spirit;

FilePtr File::create(const std::string& string)
{
    FilePtr file;

    std::string file_name, hash;
    std::size_t length;
    uint_parser<byte, 16, 2, 2> hex_char_p;
    uint_parser<std::size_t> qword_p;
    rule<> r = str_p("http://localhost:4490/GlobalFile/V1/File/") >>
        qword_p[assign_a(length)] >> ch_p('/') >>
		repeat_p(16)[hex_char_p[push_back_a(hash)]] >> ch_p('/') >>
        +anychar_p[push_back_a(file_name)];

	parse_info<> hit = parse(string.c_str(), r);
    if (hit.hit && hit.full) {
		ED2KHash ed2k_hash;
		copy(hash.begin(), hash.end(), ed2k_hash.begin());

        file.reset(new File(file_name, length, ed2k_hash));
    }

    return file;
}

File::File(const std::string& file_name, std::size_t size, const ED2KHash& hash)
: file_name_(file_name), size_(size), hash_(hash)
{
}

void File::open()
{
    /*boost::mutex::scoped_lock lock(mutex_);

    int n_block = (size_ + Block::MAX_SIZE - 1) / Block::MAX_SIZE;

	blocks_.resize(n_block);

	int level = 0;
    while (n_block > 1) {
        level++;
        n_block /= INDEX_PER_BLOCK;
    }

	process_index(root_block_, 0, level);
	*/
}

void File::close()
{
	/*
    boost::mutex::scoped_lock lock(mutex_);

    block_manager->close_file(shared_from_this());

	observers_.clear();
	*/
}

void File::read(size_t pos, size_t length, ReadCallback callback)
{
    /*
	boost::mutex::scoped_lock lock(mutex_);

	if (pos >= size_) {
		callback(NULL, 0);
		return;
	}

    BlockPos block_pos = pos / Block::MAX_SIZE;
	int offset = pos - block_pos * Block::MAX_SIZE;
	if (pos + length > size_)
		length = size_ - pos;
	if (offset + length > Block::MAX_SIZE)
		length = Block::MAX_SIZE - offset;

	observers_[block_pos].push_back(boost::bind(&File::handle_read_complete, this, _1, offset, length, callback));

	if (blocks_[block_pos]) {
		decrypted_block_cache->get_block(blocks_[block_pos],
			boost::bind(&File::handle_read_data_block, this, _1, _2, block_pos));
	}*/
}

/*
void File::handle_read_complete(const byte* buffer, int offset, int length, ReadCallback callback)
{
	callback(buffer + offset, length);
}

void File::handle_read_data_block(const byte* buffer, const int length, BlockPos block_pos)
{
	if (observers_.find(block_pos) != observers_.end()) {
		BOOST_FOREACH(Observer& observer, observers_[block_pos]) {
			observer(buffer, length);
		}
		observers_.erase(block_pos);
	}
}

void File::handle_read_index_block(const byte* buffer, const int length, BlockPos block_pos, int level)
{
	if (buffer) {
		if (level) {
			int step = 1;
			for (int i=0; i<level; i++)
				step *= INDEX_PER_BLOCK;
			for (int i=0; i<length / Block::INDEX_LENGTH; i++) {
				Block::IndexPtr index(new Block::Index);

				const byte* start = buffer + i * Block::INDEX_LENGTH;
				memmove(index->c_array(), start, index->size());
				process_index(index, block_pos + i * step, level - 1);
			}
		}
	}
}

void File::process_index(const Block::IndexPtr& index, BlockPos block_pos, int level)
{
    if (level) {
		decrypted_block_cache->get_block(index,
			boost::bind(&File::handle_read_index_block, this, _1, _2, block_pos, level));
    } else {
        if (block_pos < blocks_.size()) {
			blocks_[block_pos] = index;
			if (observers_.find(block_pos) != observers_.end()) {
				decrypted_block_cache->get_block(index,
					boost::bind(&File::handle_read_data_block, this, _1, _2, block_pos));
			}
        }
    }
}
*/
