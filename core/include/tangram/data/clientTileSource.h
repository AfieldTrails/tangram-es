#pragma once

#include "data/tileSource.h"
#include "util/types.h"

#include <mutex>

namespace Tangram {

class Platform;
struct Properties;
struct ClientTileData;

class ClientTileSource : public TileSource {

public:

    ClientTileSource(std::shared_ptr<Platform> _platform, const std::string& _name,
            const std::string& _url, bool generateCentroids = false,
            TileSource::ZoomOptions _zoomOptions = {});
    ~ClientTileSource();

    // http://www.iana.org/assignments/media-types/application/geo+json
    virtual const char* mimeType() const override { return "application/geo+json"; };

    // Add pending feature data. Pending data will be represented in tiles after
    // calling buildTiles().
    void addData(const std::string& _data);
    void addPoint(const Properties& _tags, LngLat _point);
    void addLine(const Properties& _tags, const Coordinates& _line);
    void addPoly(const Properties& _tags, const std::vector<Coordinates>& _poly);
    void generateLabelCentroidFeature();

    void buildTiles();

    virtual void loadTileData(std::shared_ptr<TileTask> _task, TileTaskCb _cb) override;
    std::shared_ptr<TileTask> createTask(TileID _tileId, int _subTask) override;

    virtual void cancelLoadingTile(const TileID& _tile) override {};
    virtual void clearData() override;

protected:

    virtual std::shared_ptr<TileData> parse(const TileTask& _task,
                                            const MapProjection& _projection) const override;

    std::unique_ptr<ClientTileData> m_store;

    mutable std::mutex m_mutexStore;
    bool m_hasPendingData = false;
    bool m_generateCentroids = false;

    std::shared_ptr<Platform> m_platform;

};

}
