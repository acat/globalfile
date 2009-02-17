#ifndef _H_FILE
#define _H_FILE

class File : public boost::enable_shared_from_this<File>,
    private boost::noncopyable {
public:
	typedef boost::array<byte, 16> ED2KHash;

private:
    //static const int INDEX_PER_BLOCK = Block::MAX_SIZE / (Block::INDEX_LENGTH);

	typedef boost::function<void (const byte*, size_t)> Observer;
	typedef boost::function<void (const byte*, size_t)> ReadCallback;

    boost::mutex  mutex_;

    std::size_t size_;
    std::string file_name_;

	ED2KHash hash_;

	//Block::IndexPtr root_block_;
	typedef int BlockPos;

	//std::vector<Block::IndexPtr> blocks_;

	boost::unordered_map<BlockPos, std::vector<Observer>> observers_;

	//void process_index(const Block::IndexPtr& index, BlockPos pos, int level);

	File(const std::string& file_name, std::size_t size, const ED2KHash& hash);

	void handle_read_complete(const byte* buffer, int offset, int length, ReadCallback callback);
	void handle_read_data_block(const byte* buffer, int length, BlockPos block_pos);
	void handle_read_index_block(const byte* buffer, int length, BlockPos block_pos, int level);
public:
    static boost::shared_ptr<File> create(const std::string& string);

    void open();
    void close();

	const std::size_t size() const {
		return size_;
	}

    void read(size_t pos, size_t length, ReadCallback callback);
};

typedef boost::shared_ptr<File> FilePtr;

#endif
