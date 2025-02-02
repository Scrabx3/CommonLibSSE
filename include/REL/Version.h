#pragma once

namespace REL
{
	class Version
	{
	public:
		using value_type = std::uint16_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		constexpr Version() noexcept = default;

		explicit constexpr Version(std::array<value_type, 4> a_version) noexcept :
			_impl(a_version)
		{}

		constexpr Version(value_type a_v1, value_type a_v2 = 0, value_type a_v3 = 0, value_type a_v4 = 0) noexcept :
			_impl{ a_v1, a_v2, a_v3, a_v4 }
		{}

		[[nodiscard]] constexpr reference       operator[](std::size_t a_idx) noexcept { return _impl[a_idx]; }
		[[nodiscard]] constexpr const_reference operator[](std::size_t a_idx) const noexcept { return _impl[a_idx]; }

		[[nodiscard]] constexpr decltype(auto) begin() const noexcept { return _impl.begin(); }
		[[nodiscard]] constexpr decltype(auto) cbegin() const noexcept { return _impl.cbegin(); }
		[[nodiscard]] constexpr decltype(auto) end() const noexcept { return _impl.end(); }
		[[nodiscard]] constexpr decltype(auto) cend() const noexcept { return _impl.cend(); }

		[[nodiscard]] std::strong_ordering constexpr compare(const Version& a_rhs) const noexcept
		{
			for (std::size_t i = 0; i < _impl.size(); ++i) {
				if ((*this)[i] != a_rhs[i]) {
					return (*this)[i] < a_rhs[i] ? std::strong_ordering::less : std::strong_ordering::greater;
				}
			}
			return std::strong_ordering::equal;
		}

		[[nodiscard]] constexpr std::uint32_t pack() const noexcept
		{
			return static_cast<std::uint32_t>(
				(_impl[0] & 0x0FF) << 24u |
				(_impl[1] & 0x0FF) << 16u |
				(_impl[2] & 0xFFF) << 4u |
				(_impl[3] & 0x00F) << 0u);
		}

		[[nodiscard]] std::string string() const
		{
			std::string result;
			for (auto&& ver : _impl) {
				result += std::to_string(ver);
				result += '-';
			}
			result.pop_back();
			return result;
		}

		[[nodiscard]] std::wstring wstring() const
		{
			std::wstring result;
			for (auto&& ver : _impl) {
				result += std::to_wstring(ver);
				result += L'-';
			}
			result.pop_back();
			return result;
		}

		[[nodiscard]] static constexpr Version unpack(std::uint32_t a_packedVersion) noexcept
		{
			return REL::Version{
				static_cast<value_type>((a_packedVersion >> 24) & 0x0FF),
				static_cast<value_type>((a_packedVersion >> 16) & 0x0FF),
				static_cast<value_type>((a_packedVersion >> 4) & 0xFFF),
				static_cast<value_type>(a_packedVersion & 0x0F)
			};
		}

	private:
		std::array<value_type, 4> _impl{ 0, 0, 0, 0 };
	};

	[[nodiscard]] constexpr bool                 operator==(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) == 0; }
	[[nodiscard]] constexpr std::strong_ordering operator<=>(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs); }

	[[nodiscard]] std::optional<Version> GetFileVersion(stl::zwstring a_filename);
}

#ifdef __cpp_lib_format
template <class CharT>
struct std::formatter<REL::Version, CharT> : formatter<std::string, CharT>
{
	template <class FormatContext>
	constexpr auto format(const REL::Version a_version, FormatContext& a_ctx) const
	{
		return formatter<std::string, CharT>::format(a_version.string(), a_ctx);
	}
};
#endif

#ifdef FMT_VERSION
template <class CharT>
struct fmt::formatter<REL::Version, CharT> : formatter<std::string, CharT>
{
	template <class FormatContext>
	auto format(const REL::Version a_version, FormatContext& a_ctx) const
	{
		return formatter<std::string, CharT>::format(a_version.string(), a_ctx);
	}
};
#endif
